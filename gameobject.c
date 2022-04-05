#include "gameobject.h"

#include <stdlib.h>

#include "print.h"

/* Notes for managing GameObject Memory
 * 
 * GameObjects stored in 128-length buffer gameObjectMemory
 * Pointers to active GameObjects stored in gameObjects buffer
 * Stack of unused gameobject addresses in gameObjectStack
 * To create new GameObject, pop address from gameObjectStack and add to end of active region of gameObjects, increment active region length
 * To destroy GameObject, set active property of gameobject to 0
 * After updates that could remove gameobjects, loop over gameObjects, 
 *  and upon finding an inactive GO, swap it with the GO at the end of active region, and decrement active region length
 */

static GameObject gameObjectMemory[MAX_GAME_OBJECTS];
static GameObject *gameObjectRefs[MAX_GAME_OBJECTS];
static int nextInactiveIndex = 0;

void initGameObjects(void) {
    for (int i = 0; i < MAX_GAME_OBJECTS; i++) {
        gameObjectRefs[i] = &gameObjectMemory[i];
    }
    nextInactiveIndex = 0;
}

GameObject *newGameObject(GameObjectType *type) {
    if (nextInactiveIndex >= MAX_GAME_OBJECTS) return NULL;
    GameObject* gameObject = gameObjectRefs[nextInactiveIndex];
    gameObject->type = type;
    gameObject->sprite = spriteAlloc();
    if (!gameObject->sprite) {
        mgba_printf("Sprite allocation failed");
        return NULL;
    }
    int initStatus = (*type->initialize)(gameObject);
    if (initStatus) {
        mgba_printf("GameObject Initialization Failed");
        spriteFree(gameObject->sprite);
        return NULL;
    }
    gameObject->active = 1;
    gameObject->lifetime = 0;
    nextInactiveIndex++;
    return gameObject;
}

void destroyGameObject(GameObject *gameObject) {
    gameObject->active = 0;
    if (gameObject->sprite) {
        gameObject->sprite->attr0 = ATTR0_HIDE;
        spriteFree(gameObject->sprite);
    }
    (*gameObject->type->destroy)(gameObject);
    return;
}

void consolidateActiveGameObjects(void) {
    for (int i = 0; i < nextInactiveIndex; i++) {
        if (!gameObjectRefs[i]->active) {
            nextInactiveIndex--;
            GameObject *temp = gameObjectRefs[i];
            gameObjectRefs[i] = gameObjectRefs[nextInactiveIndex];
            gameObjectRefs[nextInactiveIndex] = temp;
            i--;
        }
    }
}

void updateAllGameObjects(void) {
    for (int i = 0; i < nextInactiveIndex; i++) {
        GameObject *curr = gameObjectRefs[i];
        if (curr->active) {
            (*curr->type->update)(curr);
            curr->lifetime++;
        }
    }
}

void drawAllGameObjects(void) {
    for (int i = 0; i < nextInactiveIndex; i++) {
        GameObject *curr = gameObjectRefs[i];
        if (curr->active) {
            (*curr->type->draw)(curr);
        }
    }
}

void destroyAllGameObjects(void) {
    for (int i = 0; i < nextInactiveIndex; i++) {
        GameObject *curr = gameObjectRefs[i];
        if (curr->active) {
            destroyGameObject(curr);
        }
    }
    consolidateActiveGameObjects();
}