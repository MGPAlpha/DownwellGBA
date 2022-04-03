#include "HW05Lib.h"
#include "print.h"

#include "art/title.h"
#include "art/overlay.h"
#include "art/dither.h"
#include "art/instructions.h"

enum GAMESTATE {SURFACE} gameState;
int stateTime = 0;

void initSurface(void);

void updateSurface(void);

int vBlankCount = 0;

int main() {
    mgba_open();

    waitForVBlank();

    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(24) | BG_4BPP | BG_SIZE_SMALL | 2;
    REG_BG1CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_4BPP | BG_SIZE_SMALL | 0;


    DMANow(3, instructions_palette, PALETTE, INSTRUCTIONS_PALETTE_LENGTH);
    DMANow(3, instructions_tiles, &CHARBLOCK[0], INSTRUCTIONS_TILES_LENGTH);
    DMANow(3, instructions, &SCREENBLOCK[24], INSTRUCTIONS_MAP_LENGTH);
    PALETTE[17] = 0;
    DMANow(3, &dither[6*16], &CHARBLOCK[1], 16);
    unsigned short tileVal = 1<<12;
    DMANow(3, &tileVal, &SCREENBLOCK[30], 1024 | DMA_SOURCE_FIXED| DMA_16);

    DMANow(3, overlay_tiles, &CHARBLOCK[2], OVERLAY_TILES_LENGTH);
    DMANow(3, overlay, &SCREENBLOCK[31], OVERLAY_MAP_LENGTH);

    for (int i = 0; i < 30; i++) {
        waitForVBlank();
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            waitForVBlank();
        }
        DMANow(3, &dither[(6-i-1)*16], &CHARBLOCK[1], 16);
    }

    for (int i = 0; i < 120; i++) {
        waitForVBlank();
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            waitForVBlank();
        }
        DMANow(3, &dither[(i+1)*16], &CHARBLOCK[1], 16);
    }

    initSurface();

    while (1) {
        switch (gameState) {
            case SURFACE:
                updateSurface();
                break;
        }

        vBlankCount++;
        stateTime++;
    }
}

void initSurface(void) {
    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(24) | BG_4BPP | BG_SIZE_LARGE | 2; // Terrain
    REG_BG1CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_4BPP | BG_SIZE_SMALL | 0; // Dither Layer
    REG_BG2CNT = BG_CHARBLOCK(2) | BG_SCREENBLOCK(31) | BG_4BPP | BG_SIZE_SMALL | 1; // UI Overlay
    REG_BG3CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_4BPP | BG_SIZE_SMALL | 3; // Sky

    DMANow(3, title_palette, PALETTE, INSTRUCTIONS_PALETTE_LENGTH);
    PALETTE[17] = 0;
    DMANow(3, &dither[6*16], &CHARBLOCK[1], 16);
    unsigned short tileVal = 1<<12;
    DMANow(3, &tileVal, &SCREENBLOCK[30], 1024 | DMA_SOURCE_FIXED| DMA_16);

    DMANow(3, title_tiles, &CHARBLOCK[0], TITLE_TILES_LENGTH);
    DMANow(3, titlebg, &SCREENBLOCK[24], TITLEBG_MAP_LENGTH);
    DMANow(3, sky, &SCREENBLOCK[28], SKY_MAP_LENGTH);
    DMANow(3, overlay_tiles, &CHARBLOCK[2], OVERLAY_TILES_LENGTH);
    DMANow(3, overlay, &SCREENBLOCK[31], OVERLAY_MAP_LENGTH);


    for (int i = 0; i < 60; i++) {
        waitForVBlank();
    }

    gameState = SURFACE;
    stateTime = 0;
}

void updateSurface(void) {
    
    
    waitForVBlank();

    DMANow(3, &dither[MAX(0,6-stateTime/6)*16], &CHARBLOCK[1], 16);
}