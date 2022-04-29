#include "gamestate.h"
#include "HW05Lib.h"
#include "sound.h"
#include "print.h"
#include "collision.h"
#include "stdlib.h"
#include "palette.h"
#include "levelgen.h"
#include "unlocks.h"

#include "art/title.h"
// #include "art/overlay.h"
#include "art/dither.h"
#include "art/spritesheet.h"
#include "art/titlecollision.h"
#include "art/terraintiles.h"
#include "art/instructions.h"

#include "music/cavernmusic.h"
#include "sfx/logosound.h"
#include "sfx/playerkilledc50.h"

#include "player.h"
#include "camera.h"
#include "logosprite.h"
#include "overlay.h"
#include "cheats.h"
#include "gem.h"

#include "enemy.h"

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

    stopSound();

    playerHealth = 4;
    playerMaxHealth = 4;
    playerMaxHealthProgress = 0;

    waitForVBlank();

    initPalette();
    initCheats();

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
        playerData->collider.pos.y = 133;
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
                playSoundA(logosound_data, logosound_length, 0);
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
            initGame();
            return;
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

    if (playerData) {
        updateAmmoDisplay(playerData->ammo, 1);
    }

    updateHealthDisplay(playerHealth,playerMaxHealth,playerMaxHealthProgress);

    updateSprites();

    checkToEnableCheats();

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
    pauseSound();
}

void pauseFromGame(void) {
    loadMenu(&pauseMenu);
    gameState = GAME_PAUSE;
    unpauseState = GAME_PLAY;
    REG_TM2CNT = 0;
    pauseSound();
}

void unpause(void) {
    clearOverlayCenter();
    gameState = unpauseState;
    /*if (gameState == GAME_PLAY) */unpauseSound();
    REG_TM2CNT = TIMER_ON | TM_IRQ | TM_FREQ_1024;
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

int smoothCameraY = 0;

int level = 0;

int nextLevel(void) {
    destroyAllGameObjects();

    smoothCameraY = 16<<8;

    generateLevel(&startSegmentPool, &cavernSegmentPool, &endSegmentPool);

    GameObject *playerObject = newGameObject(&playerType);

    if (playerObject) {
        playerSingleton = playerObject;
        PlayerData *playerData = playerObject->data;
        playerData->collider.pos.y = 0;
        playerData->collider.pos.x = 85;
        playerData->runningJump = 1;
        playerData->state = PLAYER_JUMPING;
        playerData->stateTime = 64;

        // smoothCameraX = (playerData->collider.pos.x - SCREENWIDTH/2 + playerData->collider.size.x/2) << 8;
    }

    // spawnEnemy(&blobType, (Vector2){64,480});

    waitForVBlank();

    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE | SPRITE_MODE_2D;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(24) | BG_4BPP | BG_SIZE_LARGE | 3; // Terrain
    REG_BG2CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_4BPP | BG_SIZE_SMALL | 0; // Dither Layer

    DMANow(3, terraintiles, &CHARBLOCK[0], TERRAINTILES_LENGTH);

    clearOverlayCenter();

    generateTilemapUntil(24);

    REG_BG0HOFF = -32;
    REG_BG0VOFF = 0;

    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | SPRITE_ENABLE | SPRITE_MODE_2D;

    gameState = GAME_PLAY;
    level++;

    REG_TM3CNT = 0;
    REG_TM3D = 0;
    REG_TM3CNT = TIMER_ON | TM_IRQ | TM_CASCADE;

    REG_TM2CNT = 0;
    REG_TM2D = 65536-16384;
    REG_TM2CNT = TIMER_ON | TM_IRQ | TM_FREQ_1024;

}

void initGame(void) {

    srand(vBlankCount);

    stopSound();    

    playerHealth = 4;
    playerMaxHealth = 4;
    playerMaxHealthProgress = 0;

    initGemData();

    enemiesKilled = 0;

    level = 0;

    nextLevel();
}

void updateGame(void) {


    // if (BUTTON_PRESSED(BUTTON_L)) {
    //     PlayerData *playerData = playerSingleton->data;

    //     if (playerData) {
    //         spawnEnemy(&blobType, (Vector2){playerData->collider.pos.x + 24, playerData->collider.pos.y});
    //     }
    // }

    updateAllGameObjects();
    
    consolidateActiveGameObjects();
    

    int cameraYTarget = 0;

    PlayerData *playerData = playerSingleton ? playerSingleton->data : NULL;
    
    if (playerData && !soundA.isPlaying && (playerData->state == PLAYER_WALKING || playerData->state == PLAYER_IDLE)) {
        playSoundA(cavernmusic_data, cavernmusic_length, 1);
    }

    spawnNecessaryEnemies(playerData);

    if (playerData) {
        cameraYTarget = (playerData->collider.pos.y - SCREENHEIGHT/2 + playerData->collider.size.y/2 + 16) << 8;
        cameraYTarget = MAX(16<<8,cameraYTarget);
        cameraYTarget = MIN((currentLevelLength-11)<<12,cameraYTarget);
    }

    smoothCameraY = smoothCameraY + (cameraYTarget - smoothCameraY) / 16;

    // int invertedCameraProgress = 256 - cameraShiftProgress;

    // int cameraFollowedX = playerData->collider.pos.x + (playerData->collider.size.x - SCREENWIDTH) / 2;
    // if (cameraFollowedX < -8) cameraFollowedX = -8;


    if (playerData->state != PLAYER_DEAD) {
        cameraPos.x = -32;
        cameraPos.y = smoothCameraY>>8;
    }

    generateTilemapUntil(cameraPos.y / 16 + 11);

    if (playerData->collider.pos.y > (currentLevelLength-1)<<4) {
        nextLevel();
        return;
    }

    waitForVBlank();

    drawAllGameObjects();
    REG_BG0HOFF = cameraPos.x;
    REG_BG0VOFF = cameraPos.y;

    
    
    if (playerData) {
        updateAmmoDisplay(playerData->ammo, 1);
    }
    updateHealthDisplay(playerHealth,playerMaxHealth,playerMaxHealthProgress);
    updateGemsDisplay(gemsHeld);
    drawTimeDisplay();



    updateSprites();

    if (BUTTON_PRESSED(BUTTON_START)) {
        pauseFromGame();
    }

    if (playerData->state == PLAYER_DEAD && playerData->stateTime > 135) {
        // playSoundBPriority(playerkilledc50_data, playerkilledc50_length, 0, 200);

        // waitNVBlanks(60);
        
        initLose();
    }
}

void initLose(void) {
    REG_TM2CNT = 0;
    stopSound();
    loadMenu(&loseMenu);
    gameState = GAME_LOSE;
    waitForVBlank();
    fillOverlayRect(5, 3, 20, 14, 32*9+1);
    printToOverlay("GAME OVER", 11, 4, 0);
    
    waitNVBlanks(60);

    printToOverlay("LEVEL", 7, 6, 2);
    char levelText[4];
    sprintf(levelText, ": %d", level);
    printToOverlay(levelText, 13, 6, 0);
    waitNVBlanks(10);

    printToOverlay("GEMS", 7, 7, 2);
    char gemsText[10];
    sprintf(gemsText, ": %d", totalGemsThisRun);
    printToOverlay(gemsText, 13, 7, 0);
    waitNVBlanks(10);
    
    printToOverlay("KILLS", 7, 8, 2);
    char killsText[10];
    sprintf(killsText, ": %d", enemiesKilled);
    printToOverlay(killsText, 13, 8, 0);
    waitNVBlanks(10);

    printToOverlay("TIME", 7, 9, 2);
    short seconds = REG_TM3D;
    short minutes = seconds / 60;
    seconds %= 60;
    short centis = (REG_TM2D&~(~0<<14));
    centis = centis*100/(1<<14);
    char timeText[15];
    snprintf(timeText, 15, ": %02d:%02d.%02d", minutes, seconds, centis);
    printToOverlay(timeText, 13, 9, 0);
    
    waitNVBlanks(30);

    int nextUnlockIndex = getNextUnlockIndex(lifetimeGems);

    int gemMax = unlocks[nextUnlockIndex].quota;
    int prevGemMax = nextUnlockIndex > 0 ? unlocks[nextUnlockIndex-1].quota : 0; 
    int newLifetimeGems = lifetimeGems + totalGemsThisRun;
    int gemAccumulator = lifetimeGems;

    drawGemProgress(gemAccumulator, prevGemMax, gemMax, 11);

    waitNVBlanks(30);

    while (newLifetimeGems >= gemMax) {
        for (int i = 1; i <= 60; i++) {
            waitForVBlank();
            int gemProgress = gemAccumulator + (gemMax - gemAccumulator) * i / 60;
            drawGemProgress(gemProgress, prevGemMax, gemMax, 11);
        }
        waitNVBlanks(25);
        unlockPalette(unlocks[nextUnlockIndex].index);

        fillOverlayRect(5,11,20,3, OFFSET(1,9,32));
        printToOverlay("PALETTE UNLOCKED", 7, 11, 2);
        char *paletteName = palettes[unlocks[nextUnlockIndex].index].name;
        printToOverlay(paletteName, 15-strlen(paletteName)/2, 13, 0);
        waitNVBlanks(120);

        gemAccumulator = gemMax;
        nextUnlockIndex++;
        prevGemMax = gemMax;
        if (nextUnlockIndex >= unlockSize) break;
        gemMax = unlocks[MIN(nextUnlockIndex, 2)].quota;
    }

    for (int i = 1; i <= 60; i++) {
            waitForVBlank();
            int gemProgress = gemAccumulator + (newLifetimeGems - gemAccumulator) * i / 60;
            drawGemProgress(gemProgress, prevGemMax, gemMax, 11);
    }

    lifetimeGems = newLifetimeGems;
    setSaveInt(8, lifetimeGems);

    waitNVBlanks(60);
}

void updateLose(void) {
    updateMenu();
    waitForVBlank();
    if (gameState == GAME_LOSE) {
        drawCurrentMenu();
    }
}

void initInstructions(void) {
    gameState = GAME_INSTRUCTIONS;
    waitForVBlank();
    DMANow(3, instructions_palette, PALETTE, INSTRUCTIONS_PALETTE_LENGTH);
    DMANow(3, instructions_tiles, &CHARBLOCK[1], INSTRUCTIONS_TILES_LENGTH);
    DMANow(3, instructions, &SCREENBLOCK[29], INSTRUCTIONS_MAP_LENGTH);

    REG_BG2CNT = BG_4BPP | BG_CHARBLOCK(1) | BG_SCREENBLOCK(29) | BG_SIZE_SMALL;
    REG_DISPCTL |= BG2_ENABLE;
}

void updateInstructions(void) {
    if (oldButtons & ~buttons) {
        gameState = GAME_PAUSE;
        waitForVBlank();
        REG_DISPCTL = REG_DISPCTL & ~BG2_ENABLE;
        initPalette();
    }
}