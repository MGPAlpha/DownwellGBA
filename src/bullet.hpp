#ifndef BULLET_HPP
#define BULLET_HPP

#include "engine/gameobject.hpp"
#include "enemy.hpp"

class Bullet : public GBAEngine::Component {
    public:
        Bullet(GBAEngine::Vector2 pos);
    protected:
        void update() override;
        void draw() override;
    private:
        GBAEngine::Rect collider;
        
        unsigned int CheckCollisionWithEnemy(Enemy *enemy);
};

#endif