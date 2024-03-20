#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"

const extern GameObjectType enemyType;

typedef struct enemytype {
    int maxHealth;
    Vector2 spriteOffset;
    Vector2 colliderSize;
    void (*update)(GameObject* self);
    int (*spriteIndex)(GameObject* self);
    int maxPlayerRange;
} EnemyType;

typedef struct enemydata {
    EnemyType* type;
    Transform collider;
    Vector2 velocity;
    Vector2 frameExtraMovement;
    int health;
    int dead;
} EnemyData;

const extern EnemyType blobType;

GameObject *spawnEnemy(EnemyType *type, Vector2 pos);

void damageEnemy(GameObject *self, int damage);

void killEnemy(GameObject *self);

extern int enemiesKilled;

#endif