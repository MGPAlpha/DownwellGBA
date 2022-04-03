#include "gameobject.h"

GameObjectType gameObjectTypes[] = {};

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