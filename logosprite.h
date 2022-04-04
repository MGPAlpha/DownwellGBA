#ifndef LOGOSPRITE_H
#define LOGOSPRITE_H

#include "gameobject.h"

const extern GameObjectType logoSpriteType;

typedef struct logospritedata {
    Vector2 pos;
    int index;
    int animationStart;
} LogoSpriteData;

#endif