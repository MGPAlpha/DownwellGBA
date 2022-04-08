#include "gamestate.h"
#include "HW05Lib.h"
#include "print.h"
#include "collision.h"
#include "stdlib.h"
#include "palette.h"

#include "art/title.h"
// #include "art/overlay.h"
#include "art/dither.h"
#include "art/spritesheet.h"
#include "art/titlecollision.h"

#include "player.h"
#include "camera.h"
#include "logosprite.h"
#include "overlay.h"

enum GAMESTATE gameState;
enum GAMESTATE unpauseState;

int stateTime = 0;
int vBlankCount = 0;

static int smoothCameraX = 0;

GameObject *logoSprite0;
GameObject *logoSprite1;

int wellDescentTime = 0;

void initSurface(void) {

    destroyAllGameObjects();

    waitForVBlank();

    initPalette();

    hideSprites();

    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | BG3_ENABLE | SPRITE_ENABLE | SPRITE_MODE_2D;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(24) | BG_4BPP | BG_SIZE_LARGE | 2; // Terrain
    REG_BG2CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_4BPP | BG_SIZE_SMALL | 0; // Dither Layer
    REG_BG3CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_4BPP | BG_SIZE_SMALL | 3; // Sky

    // DMANow(3, title_palette, PALETTE, TITLE_PALETTE_LENGTH);
    // PALETTE[17] = 0;
    DMANow(3, &dither[6*16], &CHARBLOCK[1], 16);
    unsigned short tileVal = 1<<12;
    DMANow(3, &tileVal, &SCREENBLOCK[30], 1024 | DMA_SOURCE_FIXED| DMA_16);

    DMANow(3, title_tiles, &CHARBLOCK[0], TITLE_TILES_LENGTH);
    DMANow(3, titlebg, &SCREENBLOCK[24], TITLEBG_MAP_LENGTH);
    DMANow(3, sky, &SCREENBLOCK[28], SKY_MAP_LENGTH);

    waitForVBlank();
    initOverlay();
    REG_BG1CNT |= 1; // UI Overlay

    // DMANow(3, spritesheet_palette, SPRITEPALETTE, SPRITESHEET_PALETTE_LENGTH);
    DMANow(3, spritesheet, &CHARBLOCK[4], SPRITESHEET_LENGTH);

    activeCollisionMap = titlecollision;
    activeCollisionMapWidth = TITLECOLLISION_WIDTH;

    wellDescentTime = 0;

    GameObject *playerObject = newGameObject(&playerType);

    if (playerObject) {
        playerSingleton = playerObject;
        PlayerData *playerData = playerObject->data;
        playerData->collider.pos.y = 136;
        playerData->collider.pos.x = 152;
        smoothCameraX = (playerData->collider.pos.x - SCREENWIDTH/2 + playerData->collider.size.x/2) << 8;
    }

    logoSprite0 = newGameObject(&logoSpriteType);
    if (logoSprite0) {
        LogoSpriteData *logoData = logoSprite0->data;
        logoData->pos.x = 288;
        logoData->pos.y = 60;
        logoData->index = 0;
    }
    logoSprite1 = newGameObject(&logoSpriteType);
    if (logoSprite1) {
        LogoSpriteData *logoData = logoSprite1->data;
        logoData->pos.x = 354;
        logoData->pos.y = 60;
        logoData->index = 1;
    }


    for (int i = 0; i < 60; i++) {
        waitForVBlank();
    }

    gameState = GAME_SURFACE;
    stateTime = 0;
}

void updateSurface(void) {

    if (BUTTON_PRESSED(BUTTON_L)) {
        GameObject *playerObject = newGameObject(&playerType);

        if (playerObject) {
            PlayerData *playerData = playerObject->data;
            playerData->collider.pos.y = 136;
            playerData->collider.pos.x = 152;
        }
    }

    if (BUTTON_PRESSED(BUTTON_R)) {
        unlockPalette(4);
    }

    

    int playerCanMove = stateTime > 8*6;

    if (playerCanMove) updateAllGameObjects();
    
    
    consolidateActiveGameObjects();
    

    int cameraXTarget = 0;

    PlayerData *playerData = playerSingleton ? playerSingleton->data : NULL;
    if (!playerSingleton || playerData->collider.pos.x >= 260) {
        cameraXTarget = 233 << 8;
        if (logoSprite0 && logoSprite1){
            LogoSpriteData *logoData0 = logoSprite0->data;
            LogoSpriteData *logoData1 = logoSprite1->data;
            if (!logoData0->animationStart && playerCanMove && (BUTTON_HELD(BUTTON_LEFT) || BUTTON_HELD(BUTTON_RIGHT))) {
                logoData0->animationStart = logoSprite0->lifetime;
                logoData1->animationStart = logoSprite1->lifetime;
            }
        }


    } else {
        cameraXTarget = (playerData->collider.pos.x - SCREENWIDTH/2 + playerData->collider.size.x/2) << 8;
        if (playerCanMove && (BUTTON_HELD(BUTTON_LEFT) || BUTTON_HELD(BUTTON_RIGHT))) {

            if (playerData->dir == LEFT) cameraXTarget -= 48<<8;
            if (playerData->dir == RIGHT) cameraXTarget += 48<<8;
        }
        
    }

    // int invertedCameraProgress = 256 - cameraShiftProgress;

    // int cameraFollowedX = playerData->collider.pos.x + (playerData->collider.size.x - SCREENWIDTH) / 2;
    // if (cameraFollowedX < -8) cameraFollowedX = -8;


    smoothCameraX = smoothCameraX + (cameraXTarget - smoothCameraX) / 16;
    if (smoothCameraX < -8) smoothCameraX = -8;
    cameraPos.x = smoothCameraX >> 8;
    if (wellDescentTime) {
        int totalDescentTime = stateTime - wellDescentTime - 40;
        totalDescentTime = MAX(totalDescentTime, 0);
        int pan = totalDescentTime * totalDescentTime / 32;
        cameraPos.y = pan;
        if (pan > 350) {
            for (int i = 0; i < 60; i++) waitForVBlank();
            initWin();
        }
    } else {
        cameraPos.y = 0;
    }

    waitForVBlank();

    drawAllGameObjects();
    DMANow(3, &dither[MAX(0,6-stateTime/6)*16], &CHARBLOCK[1], 16);
    REG_BG0HOFF = cameraPos.x;
    REG_BG0VOFF = cameraPos.y;
    REG_BG3HOFF = cameraPos.x*5/24-40;

    updateSprites();

    if (playerCanMove && BUTTON_PRESSED(BUTTON_START)) {
        pauseFromSurface();
    } else if (!wellDescentTime && playerData && playerData->collider.pos.y > 160) {
        destroyGameObject(playerSingleton);
        wellDescentTime = stateTime;
    }
}

void pauseFromSurface(void) {
    loadMenu(&pauseMenu);
    // printToOverlay("SURFACE", 11, 3, 0);
    gameState = GAME_PAUSE;
    unpauseState = GAME_SURFACE;
}

void unpause(void) {
    clearOverlayCenter();
    gameState = unpauseState;
}

void updatePause(void) {
    updateMenu();
    waitForVBlank();
    if (gameState == GAME_PAUSE) drawCurrentMenu();
    if (BUTTON_PRESSED(BUTTON_START)) {
        unpause();
    }
}

void initWin(void) {
    fillOverlayCenter();
    printToOverlay("AT THIS POINT, THE", 5, 3, 1);
    printToOverlay("GAME PROPER WOULD", 5, 4, 1);
    printToOverlay("START. IN LIEU, THIS", 5, 5, 1);
    printToOverlay("IS THE PLACEHOLDER", 5, 6, 1);
    printToOverlay("WIN SCREEN.", 5, 7, 1);
    printToOverlay("CONGRATULATIONS!", 7, 9, 0);
    printToOverlay("YOU WIN", 11, 10, 2);
    printToOverlay("PRESS", 9, 12, 1);
    printToOverlay("START", 15, 12, 2);
    printToOverlay("TO RETURN TO", 9, 13, 1);
    printToOverlay("THE SURFACE", 9, 14, 1);
    gameState = GAME_WIN;
}

void updateWin(void) {
    if (BUTTON_PRESSED(BUTTON_START)) {
        initSurface();
    }
}