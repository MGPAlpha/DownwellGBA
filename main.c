/*
    State of the Game

    Art:
        All necessary art is pretty much there.
        A few mechanics with sprites are yet to be implemented, but the art for them is already in og-art/
    Sound:
        Looping music is in place. A bullet sound effect is played whenever the player shoots.
        A priority system is in place so a few more sound effects can be added, and can be played on condition that a more important one isn't already being played.
        Plan to add sfx for taking damage and stomping enemies.
    Changing Tiles/Map/Palette at Runtime:
        All three of these are implemented.
        The dithered fade-in effect between the instructions and the surface is achieved by changing the data for a tile.
        The overlay, menus, and game terrain are dynamically generated background maps.
        The palette system in the pause menu allows dynamically changing to one of several premade color palettes.
    Timer or Interrupt:
        I don't have a non-sound timer or interrupt implemented yet.
        [Done] I plan to add a run timer to the left side of the overlay to fulfill this requirement.
    Additional Mechanics:
        [Done] Before the final submission, I plan to add collectible gems that drop when you kill an enemy, and collecting those will unlock new palettes.
        I also may add some more level segments to be used by the level generator.
        I may add the bat enemy from the original game.
 */

#include "HW05Lib.h"
#include "print.h"
#include "gameobject.h"
#include "player.h"
#include "gamestate.h"
#include "palette.h"
#include "savedata.h"
#include "cheats.h"
#include "sound.h"
#include "gem.h"

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
    initGemData();

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
            case GAME_LOSE:
                updateLose();
                break;
            case GAME_PLAY:
                updateGame();
                break;
            case GAME_INSTRUCTIONS:
                updateInstructions();
                break;
        }

        vBlankCount++;
        stateTime++;
        oldButtons = buttons;
        buttons = BUTTONS;
    }
}

