#include "HW05Lib.h"
#include "print.h"
#include "gameobject.h"
#include "player.h"
#include "gamestate.h"
#include "palette.h"
#include "savedata.h"
#include "cheats.h"
#include "sound.h"

#include "art/title.h"
#include "art/dither.h"
#include "art/instructions.h"

int main() {
    mgba_open();

    setupSounds();
    setupInterrupts();

    initSaveData();

    initCheats();

    initSpriteMemory();
    initGameObjects();

    waitForVBlank();

    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(24) | BG_4BPP | BG_SIZE_SMALL | 2;
    REG_BG1CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_4BPP | BG_SIZE_SMALL | 0;


    DMANow(3, instructions_palette, PALETTE, INSTRUCTIONS_PALETTE_LENGTH);
    DMANow(3, instructions_tiles, &CHARBLOCK[0], INSTRUCTIONS_TILES_LENGTH);
    DMANow(3, instructions, &SCREENBLOCK[24], INSTRUCTIONS_MAP_LENGTH);
    PALETTE[17] = 0;
    DMANow(3, &dither[6*16], &CHARBLOCK[1], 16);
    unsigned short tileVal = 0;
    DMANow(3, &tileVal, &SCREENBLOCK[30], 1024 | DMA_SOURCE_FIXED| DMA_16);

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
            case GAME_SURFACE:
                updateSurface();
                break;
            case GAME_PAUSE:
                updatePause();
                break;
            case GAME_WIN:
                updateWin();
                break;
            case GAME_PLAY:
                updateGame();
                break;
        }

        vBlankCount++;
        stateTime++;
        oldButtons = buttons;
        buttons = BUTTONS;
    }
}

