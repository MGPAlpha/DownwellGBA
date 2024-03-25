#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "gameobject.hpp"

class Enemy : public Component {
    public:
        void damageEnemy(int damage);
        void killEnemy();
        Transform collider;
        Vector2 velocity;
        Vector2 frameExtraMovement;
        Vector2 spriteOffset;
        int health;
        int dead;
        fixed32 maxPlayerRange;
    protected:
        
        Enemy(Vector2 pos);
        void update() override;
        void draw() override;
    private:
        virtual int calculateSpriteIndex() = 0;

};

class BlobEnemy : public Enemy {
    public:
        BlobEnemy(Vector2 pos);
    protected:
        void update() override;
        int calculateSpriteIndex() override;
};

extern int enemiesKilled;

extern GameObjectGenerator blobGenerator;

#endif