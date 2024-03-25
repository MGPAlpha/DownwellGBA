#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <functional>
#include <list>
#include <set>
// #include <typeinfo>

#include "engine/gbamath.hpp"
#include "spritedata.hpp"

class GameObject;
class Component;

#define MAX_GAME_OBJECTS 128

class Component {
    friend class GameObject;
    public:
        GameObject* getGameObject();
    protected:

        virtual void awake();
        virtual void update();
        virtual void draw();
        virtual void destroy();

        Component();
        ~Component();

        OBJ_ATTR *sprite;
    
    private:
        GameObject* gameObject;
};

class GameObject {
    friend class Component;
    public:
        void addComponent(Component* newComponent);
        int getLifetime();
        void destroy();

        template <class ComponentType>
        ComponentType* getComponent() {
            for (Component* c : components) {
                ComponentType* result = dynamic_cast<ComponentType>(c);
                if (result) return result;
            }
            return nullptr;
        }

        static void loadGameObject(GameObject* g); 

        template <class ComponentType>
        static unsigned int doForEachGameObject(std::function<unsigned int(ComponentType)> func) {
            unsigned int outCode = 0;
            for (GameObject* go : gameObjectRefs) {
                ComponentType c = go->getComponent<ComponentType>();
                if (c) {
                    unsigned int newOut = func(c);
                    if (newOut > outCode) outCode = newOut;
                }
            }
            return outCode;
        }

    private:

        void awakeComponents();
        void updateComponents();
        void drawComponents();
        void destroyComponents();

        std::list<Component*> components;
        int lifetime = 0;

        static std::list<GameObject*> gameObjectRefs;
        static std::set<GameObject*> toBeDestroyed;

        static void updateAllGameObjects(void);
        static void drawAllGameObjects(void);
        static void destroyAllGameObjects(void);

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


#endif