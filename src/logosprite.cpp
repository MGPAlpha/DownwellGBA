#include "logosprite.hpp"

#include <cstdlib>

#include "engine/gameobject.hpp"
#include "gamestate.hpp"
#include "camera.hpp"

extern "C" {

#include "print.h"

#include "art/logo.h"

}

using namespace GBAEngine;

LogoSprite::LogoSprite(int index, Vector2 pos) {
    
    this->index = index;
    this->pos = pos;
}

void LogoSprite::draw() {

    if (!this->animationStart) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }

    int frameNum = (this->getGameObject()->getLifetime() - this->animationStart) / 6;
    frameNum = MIN(frameNum, 7);
    int yOffset = (this->getGameObject()->getLifetime() - this->animationStart) / 20;
    yOffset = MIN(yOffset, 4);
    yOffset *= -1;

    for (int i = 0; i < 3; i++) {
        int srcOffset = 16*16*i + 16*8*this->index + 16*16*4*frameNum;
        int dstOffset = 16*16+16*32*i+16*8*this->index;
        DMANow(3, logo + srcOffset, CHARBLOCK[4].tileimg + dstOffset, 16*8);
    }
    int posY = this->pos.y - cameraPos.y + yOffset;
    int posX = this->pos.x - cameraPos.x;
    if (posY < -32 || posY > 160 || posX < -64 || posY > 240) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    this->sprite->attr0 = ATTR0_REGULAR | ATTR0_WIDE | (posY) & 0x00ff;
    this->sprite->attr1 = ATTR1_LARGE | (posX) & 0x01ff;
    this->sprite->attr2 = ATTR2_PRIORITY(3) | ATTR2_TILEID(16+8*this->index,0);
}