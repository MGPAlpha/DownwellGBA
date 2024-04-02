#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "engine/gameobject.hpp"
#include "engine/transform.hpp"
#include "engine/physics.hpp"
#include "enemy.hpp"

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
        void draw() override;
        void destroy() override;

    private:
        GBAEngine::Transform* transform;
        GBAEngine::RectCollider* collider;

        static Player* singleton;
        void checkForEnemyContact(Enemy *enemy);

        
};

class PlayerPrefab : public GBAEngine::GameObject {

    public:
        PlayerPrefab(GBAEngine::Vector2);
        PlayerPrefab();

};

#endif