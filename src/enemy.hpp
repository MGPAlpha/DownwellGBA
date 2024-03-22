#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "gameobject.hpp"

const extern GameObjectType enemyType;

typedef struct enemytype {
    int maxHealth;
    Vector2 spriteOffset;
    Vector2 colliderSize;
    void (*update)(GameObject* self);
    int (*spriteIndex)(GameObject* self);
    fixed32 maxPlayerRange;
} EnemyType;

typedef struct enemydata {
    const EnemyType* type;
    Transform collider;
    Vector2 velocity;
    Vector2 frameExtraMovement;
    int health;
    int dead;
} EnemyData;

const extern EnemyType blobType;

GameObject *spawnEnemy(const EnemyType *type, Vector2 pos);

void damageEnemy(GameObject *self, int damage);

void killEnemy(GameObject *self);

extern int enemiesKilled;

#endif