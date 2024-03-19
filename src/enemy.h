#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"

const extern GameObjectType enemyType;

typedef struct enemytype {
    int maxHealth;
    Vector2 spriteOffset;
    Vector2 colliderSize;
    void (*update)(GameObject* this);
    int (*spriteIndex)(GameObject* this);
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

void damageEnemy(GameObject *this, int damage);

void killEnemy(GameObject *this);

extern int enemiesKilled;

#endif