#ifndef BULLET_HPP
#define BULLET_HPP

#include "engine/gameobject.hpp"
#include "engine/transform.hpp"
#include "engine/physics.hpp"
#include "enemy.hpp"

class Bullet : public GBAEngine::Component {
    public:
    protected:
        void awake() override;
        void update() override;
        void draw() override;
    private:
        GBAEngine::Transform* transform;
        GBAEngine::RectCollider* collider;
        
        unsigned int CheckCollisionWithEnemy(Enemy *enemy);
};

class BulletPrefab : public GBAEngine::GameObject {
    public:
        BulletPrefab(GBAEngine::Vector2 pos);
};

#endif