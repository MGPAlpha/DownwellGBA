#include "enemy.hpp"

#include <cstdlib>

#include "player.hpp"
#include "gem.hpp"
#include "camera.hpp"
#include "spritedata.hpp"
#include "collision.hpp"

int enemiesKilled = 0;

int initializeEnemy(GameObject* self) {
    EnemyData *data = (EnemyData*)malloc(sizeof(EnemyData));
    if (!data) return 1;
    data->dead = 0;
    
    self->data = data;

    return 0;
}

void updateEnemy(GameObject* self) {
    EnemyData *data = (EnemyData*)self->data;

    if (data->type->update) {
        (*data->type->update)(self);
    }

    PlayerData *playerData = (PlayerData*)playerSingleton->data;
    int playerDist = playerData->resizedCollider.pos.y - data->collider.pos.y;
    if (playerDist > data->type->maxPlayerRange) destroyGameObject(self);
}

void drawEnemy(GameObject* self) {
    EnemyData *data = (EnemyData*)self->data;
    OBJ_ATTR *sprite = self->sprite;
    int posX = (data->collider.pos.x) + data->type->spriteOffset.x - cameraPos.x;
    int posY = (data->collider.pos.y) + data->type->spriteOffset.y - cameraPos.y;

    if (posY < -16 || posY > 160 || posX < -32 || posX > 240) {
        self->sprite->attr0 = ATTR0_HIDE;
        return;
    }

    sprite->attr0 = ATTR0_REGULAR | ATTR0_WIDE | posY & 0x00ff;
    sprite->attr1 = ATTR1_MEDIUM | posX & 0x01ff;
    sprite->attr2 = (data->type->spriteIndex)(self) | ATTR2_PRIORITY(3);

}

void destroyEnemy(GameObject* self) {
    if (self->data) {
        free(self->data);
    }
}

const GameObjectType enemyType = {
    initializeEnemy,
    updateEnemy,
    drawEnemy,
    destroyEnemy
};

GameObject *spawnEnemy(const EnemyType *type, Vector2 pos) {
    GameObject *newEnemyObject = newGameObject(&enemyType);
    if (!newEnemyObject) return NULL;
    EnemyData* data = (EnemyData*)newEnemyObject->data;
    data->type = type;
    data->collider.pos.x = pos.x;
    data->collider.pos.y = pos.y;
    data->collider.size.x = type->colliderSize.x;
    data->collider.size.y = type->colliderSize.y;
    data->velocity.x = 0;
    data->velocity.y = 0;
    data->frameExtraMovement.x = 0;
    data->frameExtraMovement.y = 0;
    data->health = type->maxHealth;
    data->dead = 0;
    return newEnemyObject;
}

int getBlobSpriteIndex(GameObject *self) {
    EnemyData *data = (EnemyData*)self->data;
    return ATTR2_TILEID(self->lifetime / 6 % 4 * 4,16);
}

void updateBlob(GameObject *self) {
    EnemyData *data = (EnemyData*)self->data;

    if (playerSingleton && playerSingleton->data) {
        PlayerData *playerData = (PlayerData*)playerSingleton->data;

        Vector2 playerPos = playerData->collider.pos;
        playerPos.x += playerData->collider.size.x/2;
        playerPos.y += playerData->collider.size.y/2;

        Vector2 enemyPos = data->collider.pos;
        enemyPos.x += data->collider.size.x/2;
        enemyPos.y += data->collider.size.y/2;

        Vector2 displacement = playerPos - enemyPos;
        displacement = displacement.normalized();
        displacement = displacement / 64;

        data->velocity.x += displacement.x;
        data->velocity.y += displacement.y;

        if (data->velocity.x.value > 1<<15) data->velocity.x.value = 1<<15;
        if (data->velocity.x.value < -1<<15) data->velocity.x.value = -1<<15;
        if (data->velocity.y.value > 1<<15) data->velocity.y.value = 1<<15;
        if (data->velocity.y.value < -1<<15) data->velocity.y.value = -1<<15;

    }


    data->collider.pos.x += data->velocity.x + data->frameExtraMovement.x;
    Collision terrainCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 20);
    if (terrainCollision.collided) {
        data->collider.pos.x += terrainCollision.push.x;
        // data->velocity.x *= -1;
    }
    data->frameExtraMovement.x = 0;

    data->collider.pos.y += data->velocity.y + data->frameExtraMovement.y;
    terrainCollision = collideCollisionMap(data->collider, activeCollisionMap, activeCollisionMapWidth, 20);
    if (terrainCollision.collided) {
        data->collider.pos.y += terrainCollision.push.y;
        data->velocity.y *= -1;
    }
    data->frameExtraMovement.y = 0;
}

const EnemyType blobType = {
    3,
    {-7,-3},
    {18,11},
    updateBlob,
    getBlobSpriteIndex,
    800
};

void damageEnemy(GameObject *self, int damage) {
    EnemyData *data = (EnemyData*)self->data;
    data->health -= damage;
    data->velocity.y += 2;
    data->frameExtraMovement.y = 2;
    if (data->health <= 0) {
        killEnemy(self);
    }
}

void killEnemy(GameObject *self) {
    EnemyData *data = (EnemyData*)self->data;
    Vector2 gemSpawn = data->collider.pos + data->collider.size / 2;
    int gemCount = randRange(2,4);
    for (int i = 0; i < gemCount; i++){
        GameObject *newGem = spawnGem(gemSpawn);
        if (newGem) randomizeGem(newGem);
    }
    enemiesKilled++;
    destroyGameObject(self);
}