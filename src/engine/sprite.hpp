#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "gameobject.hpp"
#include <assetdata.hpp>
#include <unordered_map>
#include <stack>

namespace GBAEngine {

    struct OBJ_ATTR {
        unsigned short attr0;
        unsigned short attr1;
        unsigned short attr2;
        unsigned short fill;
    };
    
    class OAMManager {

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
                    uint16_t index;
                    uint16_t checkoutCount;
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
                    uint16_t index;
                    uint16_t paletteIndex;
                    uint16_t checkoutCount;
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
    };

    class SpriteRenderer : public Component {
        public:
            SpriteRenderer(Sprite* sp);
            SpriteRenderer();
        protected:
            void awake() override;
            void draw() override;
            void destroy() override;
        private:
            Sprite* currentSprite;
            OBJ_ATTR* objAttr;
    };

}
#endif