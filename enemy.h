#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"

const extern GameObjectType enemyType;

typedef struct enemydata {
    Transform collider;
} EnemyData;

#endif