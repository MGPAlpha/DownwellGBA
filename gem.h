#ifndef GEM_H
#define GEM_H

#include "gameobject.h"

const extern GameObjectType gemType;

enum GEMSTATE {GEM_PHYSICS, GEM_ATTRACT};
enum GEMTYPE {GEM_SMALL, GEM_LARGE};
enum GEMROTATION {GEM_CW, GEM_CCW};

typedef struct gemdata {
    Rect collider;
    Vector2 velocity;
    enum GEMSTATE state;
    enum GEMTYPE type;
    enum GEMROTATION rotationDirection;
} GemData;

#endif