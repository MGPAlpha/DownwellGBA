#include "sprite.hpp"
#include "../HW05Lib.hpp"
#include "camera.hpp"

#include <assets.hpp>


// Object Attribute Memory
#define OAM ((OBJ_ATTR*)(0x7000000))
// extern OBJ_ATTR shadowOAM[];

// Attribute 0
#define ATTR0_REGULAR      (0<<8)  // Normal Rendering
#define ATTR0_AFFINE       (1<<8)  // Affine Rendering
#define ATTR0_HIDE         (2<<8)  // No Rendering (Hidden)
#define ATTR0_DOUBLEAFFINE (3<<8)  // Double Affine Rendering
#define ATTR0_NOBLEND      (0<<10) // Disable Alpha Blending
#define ATTR0_BLEND        (1<<10) // Enable Alpha Blending
#define ATTR0_WINDOW       (2<<10) // Object Window Mode
#define ATTR0_MOSAIC       (1<<12) // Enable Mosaic Effect
#define ATTR0_4BPP         (0<<13)
#define ATTR0_8BPP         (1<<13)
#define ATTR0_SQUARE       (0<<14)
#define ATTR0_WIDE         (1<<14) //        |  TINY  | SMALL  | MEDIUM | LARGE  |
#define ATTR0_TALL         (2<<14) // --------------------------------------------
                                   // SQUARE |  8x8   | 16x16  | 32x32  | 64x64  |
// Attribute 1                     // --------------------------------------------
#define ATTR1_HFLIP  (1<<12)       //  WIDE  |  16x8  | 32x8   | 32x16  | 64x32  |
#define ATTR1_VFLIP  (1<<13)       // --------------------------------------------
#define ATTR1_TINY   (0<<14)       //  TALL  |  8x16  | 8x32   | 16x32  | 32x64  |
#define ATTR1_SMALL  (1<<14)       // --------------------------------------------
#define ATTR1_MEDIUM (2<<14)
#define ATTR1_LARGE  (3<<14)

// Attribute 2
#define ATTR2_TILEID(col, row) ((row)*32+(col))
#define ATTR2_PRIORITY(num)    ((num)<<10)
#define ATTR2_PALROW(row)      ((row)<<12)

namespace GBAEngine {
    
    OBJ_ATTR OAMManager::shadowOAM[128];
    std::stack<OBJ_ATTR*> OAMManager::freeOAM;

    void OAMManager::init() {
        for (int i = 0; i < 128; i++) {
            freeOAM.push(shadowOAM + i);
            freeOAM.top()->attr0 = ATTR0_HIDE;
        }
    }

    OBJ_ATTR* OAMManager::alloc() {
        if (freeOAM.size() > 0) {
            OBJ_ATTR* res = freeOAM.top();
            freeOAM.pop();
            return res;
        } 
        return nullptr;
    }

    void OAMManager::free(OBJ_ATTR* sprite) {
        sprite->attr0 = ATTR0_HIDE;
        freeOAM.push(sprite);
    }

    void OAMManager::updateSprites() {
        DMANow(3, shadowOAM, OAM, 512);
    }

    uint16_t SpritePaletteAllocator::occupiedBanks;
    std::unordered_map<const Palette16*, SpritePaletteAllocator::AllocatedPalette*> SpritePaletteAllocator::allocatedPalettes;

    SpritePaletteAllocator::AllocatedPalette* SpritePaletteAllocator::checkoutPalette(const Palette16* pal) {
        if (allocatedPalettes.count(pal) > 0) {
            auto allocated = allocatedPalettes[pal];
            allocated->checkoutCount++;
            return allocated;
        } else {
            if (occupiedBanks == 0xffff) {
                return nullptr;
            }
            int index = 0;
            for (; index < 15; index++) {
                if (!(occupiedBanks & 1 << index)) break;
            }
            auto allocated = new AllocatedPalette();
            allocated->index = index;
            allocated->checkoutCount = 1;
            DMANow(3, pal, SPRITEPALETTE, DMA_16 | DMA_ON | 16);
            allocatedPalettes[pal] = allocated;
            return allocated;
            
        }
    }
    void SpritePaletteAllocator::returnPalette(const Palette16* pal) {
        auto allocated = allocatedPalettes[pal];
        if (--allocated->checkoutCount == 0) {
            allocatedPalettes.erase(pal);
            occupiedBanks = occupiedBanks & ~(1<<allocated->index);
            delete allocated;
        }
    }


    std::unordered_map<const Sprite*, SpriteAllocator::AllocatedSprite*> SpriteAllocator::allocatedSprites;
    SpriteAllocator::SpriteMapSection* SpriteAllocator::freeList;


    void SpriteAllocator::init() {
        freeList = new SpriteMapSection();
        freeList->start = 0;
        freeList->length = 32*32;
        freeList->next = freeList;
    }

    SpriteAllocator::AllocatedSprite* SpriteAllocator::checkoutSprite(const Sprite* sp) {
        
        if (allocatedSprites.count(sp) > 0) {
            auto allocated = allocatedSprites[sp];
            allocated->checkoutCount++;
            return allocated;
        } else {
            auto allocated = allocate(sp);
            if (!allocated) return nullptr;
            allocated->checkoutCount++;
            return allocated;
        }
    }

    void SpriteAllocator::returnSprite(const Sprite* sp) {
        auto allocated = allocatedSprites[sp];
        allocated->checkoutCount--;
        if (allocated->checkoutCount == 0) {
            free(sp);
        }
    }

    void SpriteAllocator::free(const Sprite* sp) {
        // mgba_printf("freeing sprite");

        auto allocated = allocatedSprites[sp];

        uint16_t fill = 0;
        DMANow(3, &fill, &CHARBLOCK[4].tileimg[allocated->index * 16], DMA_16 | DMA_ON | DMA_SOURCE_FIXED | sp->tileCount * 16);

        if (freeList == nullptr) { // Create only node in freelist
            SpriteMapSection* newSection = new SpriteMapSection();
            newSection->start = allocated->index;
            newSection->length = sp->tileCount;
        } else {
            if (allocated->index + sp->tileCount == freeList->start) { //Check for adding to beginning of start node
                freeList->start = allocated->index;
                freeList->length += sp->tileCount;
            } else if (freeList->start + freeList->length == allocated->index) { // Check for adding to end of start node
                freeList->length += sp->tileCount;
            } else { // Cannot merge with start node, so create a new node
                SpriteMapSection* newSection = new SpriteMapSection();

                *newSection = *freeList;
                freeList->start = allocated->index;
                freeList->length = sp->tileCount;
                freeList->next = newSection;
            }
        }

        allocatedSprites.erase(sp);
        delete allocated;
        // debugFreeList();
    }

    SpriteAllocator::AllocatedSprite* SpriteAllocator::allocate(const Sprite* sp) {
        if (!freeList) return nullptr;

        auto paletteAlloc = SpritePaletteAllocator::checkoutPalette(sp->palette16);
        if (!paletteAlloc) return nullptr;

        // mgba_printf("attempting sprite alloc");

        SpriteMapSection* curr = freeList;
        SpriteMapSection* start = freeList;

        SpriteMapSection* bestNode = nullptr;
        do {
            if (curr->length >= sp->tileCount && (!bestNode || curr->length < bestNode->length)) {

                bestNode = curr;
                if (bestNode->length == sp->tileCount) {
                    break;
                } 

            }

            curr = curr->next;
        } while (curr != start);

        if (bestNode) {

            auto alloc = new SpriteAllocator::AllocatedSprite();
            alloc->index = bestNode->start;
            alloc->paletteIndex = paletteAlloc->getIndex();
            alloc->checkoutCount = 0;

            bestNode->length -= sp->tileCount;
            bestNode->start += sp->tileCount;
            freeList = bestNode;

            if (freeList->length == 0) { // Current node should be removed;

                if (freeList->next == freeList) { // No nodes will be left
                    delete freeList;
                    freeList = nullptr;
                } else if (freeList->next->next == freeList) { // One node will be left
                    auto next = freeList->next;
                    delete freeList;
                    freeList = next;
                    freeList->next = freeList;
                } else {
                    auto oldNext = freeList->next;
                    *freeList = *oldNext;
                    delete oldNext;
                }

            }

            DMANow(3, sp->data, &CHARBLOCK[4].tileimg[alloc->index * 16], DMA_16 | DMA_ON | sp->tileCount * 16);
            
            allocatedSprites[sp] = alloc;
            // debugFreeList();
            return alloc;
        }

        // this technically could miss valid sections, but awaiting logic to reorder and combine list nodes
        SpritePaletteAllocator::returnPalette(sp->palette16);
        mgba_printf("allocation failed");
        return nullptr;
    }

    void SpriteAllocator::debugFreeList() {
        auto start = freeList;
        if (!start) {
            mgba_printf("freelist is empty");
            return;
        }
        mgba_printf("freelist starts at %p", start);
        mgba_printf("traversing freelist", start);
        auto current = start;
        int largestNode = 0;
        int totalSize = 0;
        do {
            if (current->length > largestNode) largestNode = current->length;
            totalSize += current->length;
            mgba_printf("Node with index %d and size %d", current->start, current->length);
            current = current->next;
        } while (current != start);
        mgba_printf("Reached end of freelist");
        mgba_printf("Largest node has size %d", largestNode);
        mgba_printf("total freelist size %d", totalSize);
    }

    void SpriteAllocator::debugAllocatedList() {
        mgba_printf("debugging allocated sprites");
        if (allocatedSprites.size() == 0) {
            mgba_printf("no allocated sprites");
            return;
        }
        for (auto sp : allocatedSprites) {
            mgba_printf("Sprite %p allocated at %d", sp.first, sp.second->index);
        }
        
    }


    SpriteRenderer::SpriteRenderer(const Sprite* sp) {
        this->currentSprite = sp;
    }
    SpriteRenderer::SpriteRenderer() : SpriteRenderer(nullptr) {}

    void SpriteRenderer::awake() {
        this->transform = getComponent<Transform>();
        this->objAttr = OAMManager::alloc();
        if (!this->allocatedSprite && this->currentSprite) this->allocatedSprite = SpriteAllocator::checkoutSprite(this->currentSprite);
    }

    void SpriteRenderer::destroy() {
        if (this->objAttr) {
            OAMManager::free(this->objAttr);
            this->objAttr = nullptr;
        }
        if (this->allocatedSprite) {
            SpriteAllocator::returnSprite(this->currentSprite);
            this->allocatedSprite = nullptr;
        }
    }

    void SpriteRenderer::draw() {
        if (!this->objAttr) return;
        if (!this->allocatedSprite) {
            this->objAttr->attr0 = ATTR0_HIDE;
            return;
        }
        if (this->objAttr && this->allocatedSprite) {
            Vector2 anchor = this->transform->position - cameraPos + this->offset;
            int anchorX = anchor.x - this->currentSprite->pivotX;
            int anchorY = anchor.y - this->currentSprite->pivotY;
            OBJ_ATTR* obj = this->objAttr;
            obj->attr0 = ATTR0_REGULAR | this->currentSprite->shape | anchorY & 0x00ff;
            obj->attr1 = this->currentSprite->size | anchorX & 0x01ff;
            obj->attr2 = this->allocatedSprite->getIndex() | ATTR2_PRIORITY(this->renderPriority);
        }
    }

    void SpriteRenderer::setSprite(const Sprite* sp) {
        if (this->currentSprite && this->allocatedSprite) {
            SpriteAllocator::returnSprite(this->currentSprite);
            this->allocatedSprite = nullptr;
        }
        this->currentSprite = sp;
        if (this->isAwake() && sp) {
            this->allocatedSprite = SpriteAllocator::checkoutSprite(sp);
        }
    }

}

