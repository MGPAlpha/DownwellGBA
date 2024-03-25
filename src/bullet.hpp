#ifndef BULLET_HPP
#define BULLET_HPP

#include "gameobject.hpp"
#include "enemy.hpp"

class Bullet : public Component {
    public:
        Bullet(Vector2 pos);
    protected:
        void update() override;
        void draw() override;
    private:
        Rect collider;
        
        unsigned int CheckCollisionWithEnemy(Enemy *enemy);
};

#endif