#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <assetdata.hpp>
#include <unordered_map>
#include <stack>

namespace GBAEngine {
    struct OBJ_ATTR;
    class OAMManager;
    class SpriteAllocator;
    class SpritePaletteAllocator;
    class SpriteRenderer;
}

#include "gameobject.hpp"

namespace GBAEngine {

    struct OBJ_ATTR {
        unsigned short attr0;
        unsigned short attr1;
        unsigned short attr2;
        unsigned short fill;
    };
    
    class OAMManager {

        friend class SpriteRenderer;

        static void init();

        static OBJ_ATTR shadowOAM[128];
        static std::stack<OBJ_ATTR*> freeOAM;
        static OBJ_ATTR* alloc();
        static void free(OBJ_ATTR* sprite);

        static void updateSprites();

    };

    class SpritePaletteAllocator {
        public:
            struct AllocatedPalette {
                friend class SpritePaletteAllocator;
                public:
                    uint16_t getIndex() {return index;}
                private:
                    uint16_t index = -1;
                    uint16_t checkoutCount = 0;
            };

            static AllocatedPalette* checkoutPalette(const Palette16* pal);
            static void returnPalette(const Palette16* pal);
            static AllocatedPalette* checkoutPalette(const Palette256* pal);
            static void returnPalette(const Palette256* pal);
        private:
            static uint16_t occupiedBanks;
            static std::unordered_map<const Palette16*, AllocatedPalette*> allocatedPalettes;
    };

    class SpriteAllocator {
        public:
            struct AllocatedSprite {
                friend class SpriteAllocator;
                public:
                    uint16_t getIndex() {return index;}
                    uint16_t getPaletteIndex() {return paletteIndex;}
                private:
                    uint16_t index = -1;
                    uint16_t paletteIndex = -1;
                    uint16_t checkoutCount = 0;
            };
            static void init();
            static AllocatedSprite* checkoutSprite(const Sprite*);
            static void returnSprite(const Sprite*);
        private:
            struct SpriteMapSection {
                uint16_t start;
                uint16_t length;
                SpriteMapSection* next;
            };

            static SpriteMapSection* freeList;
            static std::unordered_map<const Sprite*, AllocatedSprite*> allocatedSprites;
            static AllocatedSprite* allocate(const Sprite*);
            static void free(const Sprite*);
    };

    class SpriteRenderer : public GBAEngine::Component {
        public:
            SpriteRenderer(const Sprite* sp);
            SpriteRenderer();

            void setSprite(const Sprite* sp);
        protected:
            void awake() override;
            void draw() override;
            void destroy() override;
        private:
            const Sprite* currentSprite;
            SpriteAllocator::AllocatedSprite* allocatedSprite;
            OBJ_ATTR* objAttr;
    };

}
#endif