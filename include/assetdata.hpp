#ifndef ASSETDATA_HPP
#define ASSETDATA_HPP

#include <cstdlib>

namespace GBAEngine {

    struct Palette {

    };

    typedef uint16_t Palette16[16];
    typedef uint16_t Palette256[256];

    struct Sprite {
        Palette16* palette16;
        Palette256* palette256;
        
        uint16_t* data;
        
        uint16_t sizeX;
        uint16_t sizeY;

        uint16_t bankInPalette16; 
    };

}

#endif