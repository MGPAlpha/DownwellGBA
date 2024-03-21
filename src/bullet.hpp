#ifndef BULLET_HPP
#define BULLET_HPP

#include "gameobject.hpp"

#define BULLET_SIZE_FACTOR 8

const extern GameObjectType bulletType;

typedef struct bulletdata {
    Rect collider;
} BulletData;

#endif