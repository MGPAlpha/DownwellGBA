#include "gamestate.h"
#include "HW05Lib.h"
#include "print.h"
#include "collision.h"

#include "art/title.h"
#include "art/overlay.h"
#include "art/dither.h"
#include "art/spritesheet.h"
#include "art/titlecollision.h"

#include "player.h"
#include "camera.h"

enum GAMESTATE gameState;

int stateTime = 0;
int vBlankCount = 0;

void initSurface(void) {

    waitForVBlank();

    hideSprites();

    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE | SPRITE_ENABLE | SPRITE_MODE_2D;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(24) | BG_4BPP | BG_SIZE_LARGE | 2; // Terrain
    REG_BG1CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_4BPP | BG_SIZE_SMALL | 0; // Dither Layer
    REG_BG2CNT = BG_CHARBLOCK(2) | BG_SCREENBLOCK(31) | BG_4BPP | BG_SIZE_SMALL | 1; // UI Overlay
    REG_BG3CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_4BPP | BG_SIZE_SMALL | 3; // Sky

    DMANow(3, title_palette, PALETTE, TITLE_PALETTE_LENGTH);
    PALETTE[17] = 0;
    DMANow(3, &dither[6*16], &CHARBLOCK[1], 16);
    unsigned short tileVal = 1<<12;
    DMANow(3, &tileVal, &SCREENBLOCK[30], 1024 | DMA_SOURCE_FIXED| DMA_16);

    DMANow(3, title_tiles, &CHARBLOCK[0], TITLE_TILES_LENGTH);
    DMANow(3, titlebg, &SCREENBLOCK[24], TITLEBG_MAP_LENGTH);
    DMANow(3, sky, &SCREENBLOCK[28], SKY_MAP_LENGTH);
    DMANow(3, overlay_tiles, &CHARBLOCK[2], OVERLAY_TILES_LENGTH);
    DMANow(3, overlay, &SCREENBLOCK[31], OVERLAY_MAP_LENGTH);

    DMANow(3, spritesheet_palette, SPRITEPALETTE, SPRITESHEET_PALETTE_LENGTH);
    DMANow(3, spritesheet, &CHARBLOCK[4], SPRITESHEET_LENGTH);

    activeCollisionMap = titlecollision;
    activeCollisionMapWidth = TITLECOLLISION_WIDTH;

    GameObject *playerObject = newGameObject(&playerType);

    if (playerObject) {
        PlayerData *playerData = playerObject->data;
        playerData->collider.pos.y = 136;
        playerData->collider.pos.x = 152;
    }

    for (int i = 0; i < 60; i++) {
        waitForVBlank();
    }

    gameState = GAME_SURFACE;
    stateTime = 0;
}

int cameraShiftProgress = 0;

void updateSurface(void) {

    if (BUTTON_PRESSED(BUTTON_L)) {
        GameObject *playerObject = newGameObject(&playerType);

        if (playerObject) {
            PlayerData *playerData = playerObject->data;
            playerData->collider.pos.y = 136;
            playerData->collider.pos.x = 152;
        }
    }

    updateAllGameObjects();
    consolidateActiveGameObjects();
    
    PlayerData *playerData = playerSingleton->data;

    if (playerData->collider.pos.x >= 268) {
        cameraShiftProgress -= 8;
        if (cameraShiftProgress < 0) cameraShiftProgress = 0;
    } else {
        cameraShiftProgress += 8;
        if (cameraShiftProgress > 256) cameraShiftProgress = 256;
    }

    int invertedCameraProgress = 256 - cameraShiftProgress;

    int cameraFollowedX = playerData->collider.pos.x + (playerData->collider.size.x - SCREENWIDTH) / 2;
    if (cameraFollowedX < -8) cameraFollowedX = -8;

    cameraPos.x = (cameraFollowedX * cameraShiftProgress + 232 * invertedCameraProgress) / 256;
    cameraPos.y = 0;

    waitForVBlank();

    drawAllGameObjects();
    DMANow(3, &dither[MAX(0,6-stateTime/6)*16], &CHARBLOCK[1], 16);
    REG_BG0HOFF = cameraPos.x;
    REG_BG0VOFF = cameraPos.y;
    REG_BG3HOFF = cameraPos.x*5/24-40;

    updateSprites();
}