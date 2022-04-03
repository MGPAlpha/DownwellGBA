#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "mgpmath.h"

typedef struct gameobjecttype {
    int (*initialize)(GameObject*);
    void (*update)(GameObject*);
    void (*destroy)(GameObject*);
} GameObjectType;

typedef struct gameobject {
    int active;
    GameObjectType *type;
    void *data;
    Vector2 position;
} GameObject;



#endif