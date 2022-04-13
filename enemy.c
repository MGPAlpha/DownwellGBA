#include "enemy.h"

#include "stdlib.h"
#include "camera.h"
#include "spritedata.h"

int initializeEnemy(GameObject* this) {
    EnemyData *data = malloc(sizeof(EnemyData));
    if (!data) return 1;
    data->collider.size.y = 11<<8;
    data->collider.size.x = 17<<8;
    data->dead = 0;
    
    this->data = data;

    return 0;
}

void updateEnemy(GameObject* this) {

}

void drawEnemy(GameObject* this) {
    EnemyData *data = this->data;
    OBJ_ATTR *sprite = this->sprite;
    int posX = (data->collider.pos.x >> 8) + data->type->spriteOffset.x - cameraPos.x;
    int posY = (data->collider.pos.y >> 8) + data->type->spriteOffset.y - cameraPos.y;

    if (posY < -16 || posY > 160 || posX < -32 || posX > 240) {
        this->sprite->attr0 = ATTR0_HIDE;
        return;
    }

    sprite->attr0 = ATTR0_REGULAR | ATTR0_WIDE | posY & 0x00ff;
    sprite->attr1 = ATTR1_MEDIUM | posX & 0x01ff;
    sprite->attr2 = (data->type->spriteIndex)(this) | ATTR2_PRIORITY(3);

}

void destroyEnemy(GameObject* this) {
    if (this->data) {
        free(this->data);
    }
}

const GameObjectType enemyType = {
    initializeEnemy,
    updateEnemy,
    drawEnemy,
    destroyEnemy
};

GameObject *spawnEnemy(EnemyType *type, Vector2 pos) {
    GameObject *newEnemyObject = newGameObject(&enemyType);
    if (!newEnemyObject) return NULL;
    EnemyData* data = newEnemyObject->data;
    data->type = type;
    data->collider.pos.x = pos.x<<8;
    data->collider.pos.y = pos.y<<8;
}

int getBlobSpriteIndex(GameObject *this) {
    EnemyData *data = this->data;
    return ATTR2_TILEID(this->lifetime / 6 % 4 * 4,16);
}

const EnemyType blobType = {
    {0,0},
    NULL,
    getBlobSpriteIndex
};