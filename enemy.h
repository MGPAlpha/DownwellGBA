#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"

const extern GameObjectType enemyType;

typedef struct enemytype {
    Vector2 spriteOffset;
    Vector2 colliderSize;
    void (*update)(GameObject* this);
    int (*spriteIndex)(GameObject* this);
} EnemyType;

typedef struct enemydata {
    EnemyType* type;
    Transform collider;
    Vector2 velocity;
    int dead;
} EnemyData;

const extern EnemyType blobType;

GameObject *spawnEnemy(EnemyType *type, Vector2 pos);

#endif