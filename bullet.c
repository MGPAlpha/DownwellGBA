#include "bullet.h"

#include "print.h"
#include "collision.h"

#include "camera.h"
#include "gameobject.h"
#include "stdlib.h"
#include "gamestate.h"

int bulletTravelFrames[] = {
    8,8,8,8,8,
    7,7,7,
    6,6,6,
    5,5,5,
    4,4,4,
    3,3,3,
    2,2,2,
    1,1,1
};

int initializeBullet(GameObject* this) {
    BulletData *data = malloc(sizeof(BulletData));
    if (!data) return 1;
    data->collider.pos.x = 0;
    data->collider.pos.y = 0;
    data->collider.size.x = 6;
    data->collider.size.y = 4;
    this->data = data;

    return 0;
}

void updateBullet(GameObject* this) {
    BulletData *data = this->data;
    int travelFrameIndex = this->lifetime;
    if (travelFrameIndex >= sizeof(bulletTravelFrames)/(sizeof(int))) travelFrameIndex = sizeof(bulletTravelFrames)/(sizeof(int)) - 1;
    data->collider.pos.y += bulletTravelFrames[travelFrameIndex] << BULLET_SIZE_FACTOR;
    if (this->lifetime >= 26) {
        destroyGameObject(this);
        return;
    }
    Collision groundCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 4 + BULLET_SIZE_FACTOR);
    if (groundCollision.collided) {
        destroyGameObject(this);
        return;
    }
}

void drawBullet(GameObject* this) {
    BulletData *data = this->data;
    this->sprite->attr0 = ATTR0_REGULAR | ATTR0_TALL | ((data->collider.pos.y>>BULLET_SIZE_FACTOR) - cameraPos.y - 7) & 0x00ff;
    this->sprite->attr1 = ATTR1_TINY | ((data->collider.pos.x>>BULLET_SIZE_FACTOR) - cameraPos.x - 1) & 0x01ff;
    if (this->lifetime <= 16) {
        this->sprite->attr2 = ATTR2_TILEID(0,10) | ATTR2_PRIORITY(2);
    } else {
        int aniFrame = (this->lifetime - 17) / 2 + 1;
        this->sprite->attr2 = ATTR2_TILEID(aniFrame,10) | ATTR2_PRIORITY(2);
    }
}

void destroyBullet(GameObject* this) {
    if (this->data) {
        free(this->data);
    }
}

const GameObjectType bulletType = {
    initializeBullet,
    updateBullet,
    drawBullet,
    destroyBullet
};
