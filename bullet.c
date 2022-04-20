#include "bullet.h"

#include "print.h"
#include "collision.h"

#include "camera.h"
#include "gameobject.h"
#include "stdlib.h"
#include "gamestate.h"
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
    unsigned int bulletCheckResult = doForEachGameObjectOfTypeWith(&enemyType, this, checkBulletCollisionWithEnemy);
    if (bulletCheckResult) {
        destroyGameObject(this);
        return;
    }
}

void drawBullet(GameObject* this) {
    BulletData *data = this->data;
    int posY = (data->collider.pos.y>>BULLET_SIZE_FACTOR) - cameraPos.y - 7;
    int posX = (data->collider.pos.x>>BULLET_SIZE_FACTOR) - cameraPos.x - 1;
    if (posY < -16 || posY > 160 || posX < -8 || posX > 240) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }
    this->sprite->attr0 = ATTR0_REGULAR | ATTR0_TALL | posY & 0x00ff;
    this->sprite->attr1 = ATTR1_TINY | posX & 0x01ff;
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