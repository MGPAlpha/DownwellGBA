#ifndef ASSETDATA_HPP
#define ASSETDATA_HPP

#include <cstdlib>

namespace GBAEngine {

    enum ShapeCode {
        SQUARE = 0<<0xE,
        WIDE = 1<<0xE,
        TALL = 2<<0xE
    };

    enum SizeCode {
        TINY = 0<<0xE,
        SMALL = 1<<0xE,
        MEDIUM = 2<<0xE,
        LARGE = 3<<0xE
    };

    typedef uint16_t Palette16[16];
    typedef uint16_t Palette256[256];

    struct Sprite {
        uint16_t sizeX;
        uint16_t sizeY;
        SizeCode size;
        ShapeCode shape;

        const Palette16* palette16;
        const Palette256* palette256;
        
        const uint16_t* data;
        
        uint16_t bankInPalette16 = -1; 
    };

    struct SpriteAnimation {
        const Sprite* frames;

        uint16_t frameCount;
    };

}

#endif