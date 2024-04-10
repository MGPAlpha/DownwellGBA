#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "engine/gameobject.hpp"
#include "engine/transform.hpp"
#include "engine/physics.hpp"

class Enemy : public GBAEngine::Component {
    public:
        void damageEnemy(int damage);
        void killEnemy();
        GBAEngine::Vector2 velocity;
        GBAEngine::Vector2 frameExtraMovement;
        GBAEngine::Vector2 spriteOffset;
        int health;
        int dead;
        GBAEngine::fixed32 maxPlayerRange;
    protected:
        
        Enemy();
        void awake() override;
        void update() override;
        void draw() override;
    private:
        GBAEngine::Transform* transform;
        GBAEngine::RectCollider* collider;

        virtual int calculateSpriteIndex() = 0;

};

class BlobEnemy : public Enemy {
    public:
        BlobEnemy();
    protected:
        void awake() override;
        void update() override;
        int calculateSpriteIndex() override;
    private:
        GBAEngine::Transform* transform;
        GBAEngine::RectCollider* collider;
};

class BlobPrefab : public GBAEngine::GameObject {
    public:
        BlobPrefab(GBAEngine::Vector2 pos);
};

extern int enemiesKilled;

extern GBAEngine::GameObjectGenerator blobGenerator;

#endif