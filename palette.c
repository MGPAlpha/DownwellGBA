#include "palette.h"

#include "HW05Lib.h"

int activePalette;

Palette palettes[] = {
    {{COLOR(30,30,30),COLOR(30,0,0), COLOR(0,15,30), COLOR(1,1,1)}},
    {{COLOR(30,30,30),COLOR(16,22,0), COLOR(0,15,30), COLOR(1,1,1)}},
    {{COLOR(30,30,30),COLOR(0,15,30), COLOR(30,0,0), COLOR(1,1,1)}}
};

void loadPalette(int index) {
    PALETTE[1] = palettes[index].colors[3];
    PALETTE[2] = palettes[index].colors[0];
    PALETTE[3] = palettes[index].colors[1];

    PALETTE[1+16] = palettes[index].colors[3];
    PALETTE[2+16] = palettes[index].colors[1];
    PALETTE[3+16] = palettes[index].colors[0];

    SPRITEPALETTE[1] = palettes[index].colors[3];
    SPRITEPALETTE[2] = palettes[index].colors[1];
    SPRITEPALETTE[3] = palettes[index].colors[0];

    activePalette = index;
}

int getCurrPalette(void) {
    return activePalette;
}