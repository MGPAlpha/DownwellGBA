#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "mgpmath.h"
#include "spritedata.h"

#define MAX_GAME_OBJECTS 128

typedef struct gameobjecttype {
    int (*initialize)(struct gameobject*);
    void (*update)(struct gameobject*);
    void (*draw)(struct gameobject*);
    void (*destroy)(struct gameobject*);
} GameObjectType;

typedef struct gameobject {
    int active;
    GameObjectType *type;
    void *data;
    OBJ_ATTR *sprite;
    int lifetime;
} GameObject;

void initGameObjects(void);

GameObject *newGameObject(GameObjectType *type);

void destroyGameObject(GameObject *gameObject);

void consolidateActiveGameObjects(void);

void updateAllGameObjects(void);
void drawAllGameObjects(void);

#endif