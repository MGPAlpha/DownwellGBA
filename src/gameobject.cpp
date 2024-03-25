#include "gameobject.hpp"

#include <cstdlib>
#include <functional>

extern "C" {

#include "print.h"

}

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

Component::Component() {
    this->sprite = spriteAlloc();
    if (!this->sprite) {
        mgba_printf("Sprite allocation failed");
        // return NULL;
    }
}

void Component::awake() {
    
}
void Component::update() {
    
}
void Component::draw() {
    
}
void Component::destroy() {
    
}


// void initGameObjects(void) {
//     for (int i = 0; i < MAX_GAME_OBJECTS; i++) {
//         gameObjectRefs[i] = &gameObjectMemory[i];
//     }
//     nextInactiveIndex = 0;
// }

void GameObject::addComponent(Component* newComponent) {
    components.push_back(newComponent);
    newComponent->gameObject = this;
}

int GameObject::getLifetime() {
    return this->lifetime;
}

void GameObject::awakeComponents() {
    for (Component* c : this->components) {
        c->awake();
    }
}
void GameObject::updateComponents() {
    for (Component* c : this->components) {
        c->update();
    }
    this->lifetime++;
}
void GameObject::drawComponents() {
    for (Component* c : this->components) {
        c->draw();
    }
}
void GameObject::destroyComponents() {
    for (Component* c : this->components) {
        c->destroy();
        delete c;
    }
    components.clear();
}

void GameObject::destroy() {
    toBeDestroyed.insert(this);
}

// void consolidateActiveGameObjects(void) {
//     for (int i = 0; i < nextInactiveIndex; i++) {
//         if (!gameObjectRefs[i]->active) {
//             nextInactiveIndex--;
//             GameObject *temp = gameObjectRefs[i];
//             gameObjectRefs[i] = gameObjectRefs[nextInactiveIndex];
//             gameObjectRefs[nextInactiveIndex] = temp;
//             i--;
//         }
//     }
// }

void GameObject::loadGameObject(GameObject* g) {
    gameObjectRefs.push_back(g);
    g->awakeComponents();
} 

void GameObject::updateAllGameObjects(void) {
    for (GameObject* curr : gameObjectRefs) {
        // if (curr->active) {
        curr->updateComponents();
        // }
    }
}

void GameObject::drawAllGameObjects(void) {
    for (GameObject* curr : gameObjectRefs) {
        // if (curr->active) {
        curr->drawComponents();
        // }
    }
}

void GameObject::destroyAllGameObjects(void) {
    for (GameObject* curr : gameObjectRefs) {
        // if (curr->active) {
        curr->destroyComponents();
        delete curr;
        // }
    }
    gameObjectRefs.clear();
    toBeDestroyed.clear();
}