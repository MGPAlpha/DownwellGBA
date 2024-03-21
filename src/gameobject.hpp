#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <functional>

extern "C" {

#include "mgpmath.h"
#include "spritedata.h"

}

#define MAX_GAME_OBJECTS 128

typedef struct gameobjecttype {
    int (*initialize)(struct gameobject*);
    void (*update)(struct gameobject*);
    void (*draw)(struct gameobject*);
    void (*destroy)(struct gameobject*);
} GameObjectType;

typedef struct gameobject {
    int active;
    const GameObjectType *type;
    void *data;
    OBJ_ATTR *sprite;
    int lifetime;
} GameObject;

void initGameObjects(void);

GameObject *newGameObject(const GameObjectType *type);

void destroyGameObject(GameObject *gameObject);

void consolidateActiveGameObjects(void);

void updateAllGameObjects(void);
void drawAllGameObjects(void);
void destroyAllGameObjects(void);

unsigned int doForEachGameObjectOfTypeWith(const GameObjectType* type, void *with, std::function<unsigned int(GameObject*, void*)> func);

#endif