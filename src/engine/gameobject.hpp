#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <functional>
#include <list>
#include <set>
// #include <typeinfo>

#include "gbamath.hpp"
#include "../spritedata.hpp"

namespace GBAEngine {

class GameObject;
class Component;

#define MAX_GAME_OBJECTS 128

class GameObject {
    friend class Component;
    public:
        void addComponent(Component* newComponent);
        static int getSceneLifetime();
        int getLifetime();
        void destroy();

        template <class ComponentType>
        ComponentType* getComponent() {
            for (Component* c : components) {
                ComponentType* result = dynamic_cast<ComponentType*>(c);
                if (result) return result;
            }
            return nullptr;
        }

        static void loadGameObject(GameObject* g); 

        // template <class ComponentType>
        // static unsigned int doForEachGameObject(std::function<unsigned int(ComponentType*)> func) {
        //     unsigned int outCode = 0;
        //     for (GameObject* go : gameObjectRefs) {
        //         ComponentType* c = go->getComponent<ComponentType>();
        //         if (c) {
        //             unsigned int newOut = func(c);
        //             if (newOut > outCode) outCode = newOut;
        //         }
        //     }
        //     return outCode;
        // }
        static void updateAllGameObjects(void);
        static void lateUpdateAllGameObjects(void);
        static void drawAllGameObjects(void);
        static void destroyAllGameObjects(void);
        static void clearDestructionQueue();

    private:

        void awakeComponents();
        void updateComponents();
        void lateUpdateComponents();
        void drawComponents();
        void destroyComponents();

        std::list<Component*> components;
        int lifetime = 0;

        static std::list<GameObject*> gameObjectRefs;
        static std::set<GameObject*> toBeDestroyed;

        static int sceneLifetime;

};

class Component {
    friend class GameObject;
    public:
        GameObject* getGameObject();

        template <class ComponentType>
        ComponentType* getComponent() {
            if (!gameObject) return nullptr;
            return gameObject->getComponent<ComponentType>();
        }
    protected:

        virtual void awake();
        virtual void lateUpdate();
        virtual void update();
        virtual void draw();
        virtual void destroy();

        Component();
        virtual ~Component();

        OBJ_ATTR *sprite;
    
    private:
        GameObject* gameObject;
};

// typedef struct gameobjecttype {
//     int (*initialize)(struct gameobject*);
//     void (*update)(struct gameobject*);
//     void (*draw)(struct gameobject*);
//     void (*destroy)(struct gameobject*);
// } GameObjectType;

// typedef struct gameobject {
//     int active;
//     const GameObjectType *type;
//     void *data;
//     OBJ_ATTR *sprite;
//     int lifetime;
// } GameObject;

void initGameObjects(void);

// GameObject *newGameObject(const GameObjectType *type);


void consolidateActiveGameObjects(void);

class GameObjectGenerator {
    public:
        GameObjectGenerator(std::function<GameObject*(GBAEngine::Vector2)> genFunc);
        GameObject* operator() (GBAEngine::Vector2 v) const;
    private:
        std::function<GameObject*(GBAEngine::Vector2)> generatorFunction;
};

}

#endif