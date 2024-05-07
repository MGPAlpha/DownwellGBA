#include "gamestate.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "cheats.hpp"
#include "palette.hpp"
#include "overlay.hpp"
#include "player.hpp"
#include "gem.hpp"
#include "logosprite.hpp"
#include "enemy.hpp"
#include "levelgen.hpp"
#include "savedata.hpp"
#include "engine/camera.hpp"
#include "HW05Lib.hpp"
#include "sound.hpp"
#include "collision.hpp"

#include "engine/sprite.hpp"
#include "../srcgen/assets.hpp"

extern "C" {
    
#include "print.h"


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



}

using namespace GBAEngine;

enum GAMESTATE gameState;
enum GAMESTATE unpauseState;

int stateTime = 0;
int vBlankCount = 0;

LogoSprite *logoSprite0;
LogoSprite *logoSprite1;

Camera* titleCamComponent;
MovementAnimator* titleCamAnimator;

int wellDescentTime = 0;

void initSurface(void) {

    GameObject::destroyAllGameObjects();

    stopSound();

    Player::playerHealth = 4;
    Player::playerMaxHealth = 4;
    Player::playerMaxHealthProgress = 0;

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

    activeCollisionMap = (char*)titlecollision;
    activeCollisionMapWidth = TITLECOLLISION_WIDTH;

    wellDescentTime = 0;

    GameObject* player = new PlayerPrefab(Vector2(152,133));
    Camera* playerCam = new Camera();
    playerCam->damping = 15;
    playerCam->lockPos.y = 80;
    playerCam->lockY = true;
    playerCam->clampX = true;
    playerCam->clampMin.x = 112;
    playerCam->clampMax.x = 300;
    player->addComponent(playerCam);
    Transform* playerTransform = player->getComponent<Transform>();
    Player* playerComponent = player->getComponent<Player>();
    GameObject::loadGameObject(player);

    GameObject* logoSpriteObj0 = new GameObject();
    logoSprite0 = new LogoSprite(0);
    Transform* logoSpriteTransform0 = new Transform(Vector2(288, 60));
    logoSpriteObj0->addComponent(logoSprite0);
    logoSpriteObj0->addComponent(logoSpriteTransform0);
    GameObject::loadGameObject(logoSpriteObj0);

    GameObject* logoSpriteObj1 = new GameObject();
    logoSprite1 = new LogoSprite(1);
    Transform* logoSpriteTransform1 = new Transform(Vector2(354, 60));
    logoSpriteObj1->addComponent(logoSprite1);
    logoSpriteObj1->addComponent(logoSpriteTransform1);
    GameObject::loadGameObject(logoSpriteObj1);

    GameObject* titleCam = new GameObject();
    titleCamComponent = new Camera(5, false);
    titleCam->addComponent(new Transform(Vector2(352, 80)));
    titleCam->addComponent(titleCamComponent);
    titleCamAnimator = new MovementAnimator(Vector2(0,350), 106, EasingFunction::EASE_IN);
    titleCam->addComponent(titleCamAnimator);
    GameObject::loadGameObject(titleCam);

    for (int i = 0; i < 60; i++) {
        waitForVBlank();
    }

    gameState = GAME_SURFACE;
    stateTime = 0;
    CameraSystem::resetToPrimaryCam();
}

void updateSurface(void) {

    

    int playerCanMove = stateTime > 8*6;

    if (playerCanMove) GameObject::updateAllGameObjects();

    Physics::updateIntersections();
    
    GameObject::lateUpdateAllGameObjects();
    
    GameObject::clearDestructionQueue();
    // consolidateActiveGameObjects();
    
    CameraSystem::update();

    Player *player = Player::getSingleton();
    Camera* playerCamera;
    if (player) {
        playerCamera = player->getComponent<Camera>();
    }
    if (!player || player->getTransform()->position.x >= 260) {
        if (player) {
            playerCamera->offset.x = 0;
            titleCamComponent->enable();
        }
        if (logoSprite0 && logoSprite1){
            if (!logoSprite0->animationStart && playerCanMove && (BUTTON_HELD(BUTTON_LEFT) || BUTTON_HELD(BUTTON_RIGHT))) {
                logoSprite0->animationStart = logoSprite0->getGameObject()->getLifetime();
                logoSprite1->animationStart = logoSprite1->getGameObject()->getLifetime();
                playSoundA(logosound_data, logosound_length, 0);
            }
        }


    } else {
        titleCamComponent->disable();
        if (playerCanMove && (BUTTON_HELD(BUTTON_LEFT) || BUTTON_HELD(BUTTON_RIGHT))) {

            if (player->dir == LEFT) playerCamera->offset.x = -48;
            else if (player->dir == RIGHT) playerCamera->offset.x = 48;
        } else playerCamera->offset.x = 0;
        
    }

    if (wellDescentTime) {
        int totalDescentTime = stateTime - wellDescentTime - 40;
        totalDescentTime = MAX(totalDescentTime, 0);
        int pan = totalDescentTime * totalDescentTime / 32;
        if (pan > 350) {
            for (int i = 0; i < 60; i++) waitForVBlank();
            mgba_printf("initiating game");
            initGame();
            return;
        }
    }

    waitForVBlank();

    GameObject::drawAllGameObjects();
    DMANow(3, &dither[MAX(0,6-stateTime/6)*16], &CHARBLOCK[1], 16);
    REG_BG0HOFF = int(cameraPos.x);
    REG_BG0VOFF = int(cameraPos.y);
    REG_BG3HOFF = int(cameraPos.x*5/24-40);

    if (player) {
        updateAmmoDisplay(player->ammo, 1);
    }

    updateHealthDisplay(Player::playerHealth,Player::playerMaxHealth,Player::playerMaxHealthProgress);

    updateSprites();

    checkToEnableCheats();

    if (playerCanMove && BUTTON_PRESSED(BUTTON_START)) {
        pauseFromSurface();
    } else if (!wellDescentTime && player && player->getTransform()->position.y > 160) {
        mgba_printf("destroying player");
        player->getGameObject()->destroy();
        mgba_printf("player destroyed"); 
        wellDescentTime = stateTime;
        titleCamAnimator->start();
        mgba_printf("wellDescentTime: %d", wellDescentTime); 
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
    printToOverlay("AT self POINT, THE", 5, 3, 1);
    printToOverlay("GAME PROPER WOULD", 5, 4, 1);
    printToOverlay("START. IN LIEU, self", 5, 5, 1);
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

int level = 0;

void nextLevel(void) {
    GameObject::destroyAllGameObjects();

    generateLevel(&startSegmentPool, &cavernSegmentPool, &endSegmentPool);

    GameObject *player = new PlayerPrefab(Vector2(85, 0));
    Camera* playerCamera = new Camera();
    playerCamera->offset.y = 16;
    playerCamera->lockX = true;
    playerCamera->lockPos.x = 88;
    playerCamera->damping = 15;
    playerCamera->clampY = true;
    playerCamera->clampMin.y = 96;
    playerCamera->clampMax.y = (currentLevelLength-6)<<4;
    player->addComponent(playerCamera);
    Player* playerComponent = player->getComponent<Player>();
    GameObject::loadGameObject(player);

    if (playerComponent) {
        playerComponent->runningJump = 1;
        playerComponent->state = PLAYER_JUMPING;
        playerComponent->stateTime = 64;
    }

    mgba_printf("new player generated");


    waitForVBlank();

    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE | SPRITE_MODE_2D;

    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(24) | BG_4BPP | BG_SIZE_LARGE | 3; // Terrain
    REG_BG2CNT = BG_CHARBLOCK(1) | BG_SCREENBLOCK(30) | BG_4BPP | BG_SIZE_SMALL | 0; // Dither Layer

    DMANow(3, terraintiles, &CHARBLOCK[0], TERRAINTILES_LENGTH);

    mgba_printf("terrain tiles loaded");

    clearOverlayCenter();

    mgba_printf("overlay cleared");

    generateTilemapUntil(24);

    mgba_printf("tilemap generated");

    REG_BG0HOFF = -32;
    REG_BG0VOFF = 0;

    REG_DISPCTL = MODE0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | SPRITE_ENABLE | SPRITE_MODE_2D;

    gameState = GAME_PLAY;

    level++;

    CameraSystem::resetToPrimaryCam();


}

void initGame(void) {

    srand(vBlankCount);

    stopSound();    

    GameObject::clearDestructionQueue();


    Player::playerHealth = 4;
    Player::playerMaxHealth = 4;
    Player::playerMaxHealthProgress = 0;

    initGemData();

    enemiesKilled = 0;

    level = 0;

    nextLevel();

    REG_TM3CNT = 0;
    REG_TM3D = 0;
    REG_TM3CNT = TIMER_ON | TM_IRQ | TM_CASCADE;

    REG_TM2CNT = 0;
    REG_TM2D = 65536-16384;
    REG_TM2CNT = TIMER_ON | TM_IRQ | TM_FREQ_1024;
}

void updateGame(void) {


    GameObject::updateAllGameObjects();

    Physics::updateIntersections();

    GameObject::lateUpdateAllGameObjects();
    
    GameObject::clearDestructionQueue();

    CameraSystem::update();

    Player *player = Player::getSingleton();
    
    if (player && !soundA.isPlaying && (player->state == PLAYER_WALKING || player->state == PLAYER_IDLE)) {
        playSoundA(cavernmusic_data, cavernmusic_length, 1);
    }

    spawnNecessaryEnemies(player);


    generateTilemapUntil(int(cameraPos.y) / 16 + 11);

    if (player->state != PLAYER_DEAD && player->getTransform()->position.y > (currentLevelLength-1)<<4) {
        nextLevel();
        return;
    }

    waitForVBlank();

    GameObject::drawAllGameObjects();
    REG_BG0HOFF = int(cameraPos.x);
    REG_BG0VOFF = int(cameraPos.y);

    
    
    if (player) {
        updateAmmoDisplay(player->ammo, 1);
    }
    updateHealthDisplay(Player::playerHealth,Player::playerMaxHealth,Player::playerMaxHealthProgress);
    updateGemsDisplay(gemsHeld);
    drawTimeDisplay();



    updateSprites();

    checkToEnableCheats();

    if (player->state != PLAYER_DEAD && BUTTON_PRESSED(BUTTON_START)) {
        pauseFromGame();
    }

    if (player->state == PLAYER_DEAD && player->stateTime > 135) {
        // playSoundBPriority(playerkilledc50_data, playerkilledc50_length, 0, 200);

        // waitNVBlanks(60);
        
        initLose();
    }
}

void initLose(void) {
    REG_TM2CNT = 0;
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