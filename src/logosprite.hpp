#ifndef LOGOSPRITE_HPP
#define LOGOSPRITE_HPP

#include "gameobject.hpp"

const extern GameObjectType logoSpriteType;

typedef struct logospritedata {
    Vector2 pos;
    int index;
    int animationStart;
} LogoSpriteData;

#endif