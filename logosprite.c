#include "logosprite.h"

#include "print.h"

#include "art/logo.h"

#include "camera.h"
#include "gameobject.h"
#include "stdlib.h"

int initializeLogoSprite(GameObject* this) {
    LogoSpriteData *data = malloc(sizeof(LogoSpriteData));
    if (!data) return 1;
    this->data = data;

    return 0;
}

void updateLogoSprite(GameObject* this) {
    LogoSpriteData *data = this->data;
    
}

void drawLogoSprite(GameObject* this) {
    LogoSpriteData *data = this->data;
    for (int i = 0; i < 3; i++) {
        int srcOffset = 16*16*i + 16*8*data->index;
        int dstOffset = 16*16+16*32*i+16*8*data->index;
        mgba_printf("Src: %x, Dst: %x", srcOffset, dstOffset);
        DMANow(3, logo + srcOffset, CHARBLOCK[4].tileimg + dstOffset, 16*8);
    }
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
