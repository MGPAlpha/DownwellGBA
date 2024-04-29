#ifndef ASSETDATA_HPP
#define ASSETDATA_HPP

#include <cstdlib>

namespace GBAEngine {

    typedef uint16_t Palette16[16];
    typedef uint16_t Palette256[256];

    struct Sprite {
        uint16_t sizeX;
        uint16_t sizeY;

        const Palette16* palette16;
        const Palette256* palette256;
        
        const uint16_t* data;
        
        uint16_t bankInPalette16 = -1; 
    };

}

#endif