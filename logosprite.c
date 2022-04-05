#include "logosprite.h"

#include "print.h"

#include "art/logo.h"

#include "camera.h"
#include "gameobject.h"
#include "stdlib.h"
#include "gamestate.h"

int initializeLogoSprite(GameObject* this) {
    LogoSpriteData *data = malloc(sizeof(LogoSpriteData));
    if (!data) return 1;
    data->animationStart = 0;
    this->data = data;

    return 0;
}

void updateLogoSprite(GameObject* this) {
    LogoSpriteData *data = this->data;
    
}

void drawLogoSprite(GameObject* this) {
    LogoSpriteData *data = this->data;
    

    if (!data->animationStart) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }

    int frameNum = (this->lifetime - data->animationStart) / 6;
    frameNum = MIN(frameNum, 7);
    int yOffset = (this->lifetime - data->animationStart) / 20;
    yOffset = MIN(yOffset, 4);
    yOffset *= -1;

    for (int i = 0; i < 3; i++) {
        int srcOffset = 16*16*i + 16*8*data->index + 16*16*4*frameNum;
        int dstOffset = 16*16+16*32*i+16*8*data->index;
        DMANow(3, logo + srcOffset, CHARBLOCK[4].tileimg + dstOffset, 16*8);
    }
    int posY = data->pos.y - cameraPos.y + yOffset;
    int posX = data->pos.x - cameraPos.x;
    if (posY < -32 || posY > 160 || posX < -64 || posY > 240) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    this->sprite->attr0 = ATTR0_REGULAR | ATTR0_WIDE | (posY) & 0x00ff;
    this->sprite->attr1 = ATTR1_LARGE | (posX) & 0x01ff;
    this->sprite->attr2 = ATTR2_PRIORITY(2) | ATTR2_TILEID(16+8*data->index,0);
}

void destroyLogoSprite(GameObject* this) {
    if (this->data) {
        free(this->data);
    }
}

const GameObjectType logoSpriteType = {
    initializeLogoSprite,
    updateLogoSprite,
    drawLogoSprite,
    destroyLogoSprite
};
