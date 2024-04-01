#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "engine/gameobject.hpp"

class Enemy : public GBAEngine::Component {
    public:
        void damageEnemy(int damage);
        void killEnemy();
        GBAEngine::Rect collider;
        GBAEngine::Vector2 velocity;
        GBAEngine::Vector2 frameExtraMovement;
        GBAEngine::Vector2 spriteOffset;
        int health;
        int dead;
        GBAEngine::fixed32 maxPlayerRange;
    protected:
        
        Enemy(GBAEngine::Vector2 pos);
        void update() override;
        void draw() override;
    private:
        virtual int calculateSpriteIndex() = 0;

};

class BlobEnemy : public Enemy {
    public:
        BlobEnemy(GBAEngine::Vector2 pos);
    protected:
        void update() override;
        int calculateSpriteIndex() override;
};

extern int enemiesKilled;

extern GBAEngine::GameObjectGenerator blobGenerator;

#endif