#include "bullet.h"

#include "print.h"
#include "collision.h"

#include "camera.h"
#include "gameobject.h"
#include "stdlib.h"
#include "gamestate.hpp"
#include "enemy.h"

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

unsigned int checkBulletCollisionWithEnemy(GameObject *enemy, GameObject *bullet) {
    BulletData *bulletData = bullet->data;
    EnemyData *enemyData = enemy->data;

    Collision testCollision = collideRects(bulletData->collider, enemyData->collider);

    if (testCollision.collided) {
        damageEnemy(enemy, 1);
        return 1;
    }
    return 0;
}

int initializeBullet(GameObject* self) {
    BulletData *data = malloc(sizeof(BulletData));
    if (!data) return 1;
    data->collider.pos.x = 0;
    data->collider.pos.y = 0;
    data->collider.size.x = 6;
    data->collider.size.y = 4;
    self->data = data;

    return 0;
}

void updateBullet(GameObject* self) {
    BulletData *data = self->data;
    int travelFrameIndex = self->lifetime;
    if (travelFrameIndex >= sizeof(bulletTravelFrames)/(sizeof(int))) travelFrameIndex = sizeof(bulletTravelFrames)/(sizeof(int)) - 1;
    data->collider.pos.y += bulletTravelFrames[travelFrameIndex] << BULLET_SIZE_FACTOR;
    if (self->lifetime >= 26) {
        destroyGameObject(self);
        return;
    }
    Collision groundCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 4 + BULLET_SIZE_FACTOR);
    if (groundCollision.collided) {
        destroyGameObject(self);
        return;
    }
    unsigned int bulletCheckResult = doForEachGameObjectOfTypeWith(&enemyType, self, checkBulletCollisionWithEnemy);
    if (bulletCheckResult) {
        destroyGameObject(self);
        return;
    }
}

void drawBullet(GameObject* self) {
    BulletData *data = self->data;
    int posY = (data->collider.pos.y>>BULLET_SIZE_FACTOR) - cameraPos.y - 7;
    int posX = (data->collider.pos.x>>BULLET_SIZE_FACTOR) - cameraPos.x - 1;
    if (posY < -16 || posY > 160 || posX < -8 || posX > 240) {
        self->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    self->sprite->attr0 = ATTR0_REGULAR | ATTR0_TALL | posY & 0x00ff;
    self->sprite->attr1 = ATTR1_TINY | posX & 0x01ff;
    if (self->lifetime <= 16) {
        self->sprite->attr2 = ATTR2_TILEID(0,10) | ATTR2_PRIORITY(2);
    } else {
        int aniFrame = (self->lifetime - 17) / 2 + 1;
        self->sprite->attr2 = ATTR2_TILEID(aniFrame,10) | ATTR2_PRIORITY(2);
    }
}

void destroyBullet(GameObject* self) {
    if (self->data) {
        free(self->data);
    }
}

const GameObjectType bulletType = {
    initializeBullet,
    updateBullet,
    drawBullet,
    destroyBullet
};