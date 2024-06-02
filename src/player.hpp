#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "engine/gameobject.hpp"
#include "engine/transform.hpp"
#include "engine/physics.hpp"
#include "engine/sprite.hpp"
#include "engine/animation.hpp"
#include "enemy.hpp"
#include "../srcgen/assets.hpp"

// const extern GameObjectType playerType;

enum PLAYERSTATE {PLAYER_IDLE, PLAYER_WALKING, PLAYER_JUMPING, PLAYER_HOP, PLAYER_FALLING, PLAYER_DEAD};

class Player : public GBAEngine::Component {
    public:
        Player();

        GBAEngine::Transform* getTransform();
        GBAEngine::RectCollider* getCollider();


        static Player* getSingleton();
        static int playerHealth;
        static int playerMaxHealth;
        static int playerMaxHealthProgress;
        enum PLAYERSTATE state;
        GBAEngine::Vector2 velocity;
        int stateTime;
        enum DIRECTION dir;
        int runningJump;
        int canFire;
        int fireTime;
        int charge;
        int ammo;
        int iFrames;
    protected:
        void awake() override;
        void update() override;
        void destroy() override;

    private:
        GBAEngine::Transform* transform;
        GBAEngine::RectCollider* collider;
        GBAEngine::SpriteRenderer* spriteRenderer;
        GBAEngine::SpriteAnimator* spriteAnimator;

        static Player* singleton;
        void checkForEnemyContact(Enemy *enemy);

        
};

class PlayerPrefab : public GBAEngine::GameObject {

    public:
        PlayerPrefab(GBAEngine::Vector2);
        PlayerPrefab();

};

#endif