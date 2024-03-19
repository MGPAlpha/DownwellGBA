#ifndef BULLET_H
#define BULLET_H

#include "gameobject.h"

#define BULLET_SIZE_FACTOR 8

const extern GameObjectType bulletType;

typedef struct bulletdata {
    Rect collider;
} BulletData;

#endif