#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "engine/gameobject.hpp"
#include "enemy.hpp"

// const extern GameObjectType playerType;

enum PLAYERSTATE {PLAYER_IDLE, PLAYER_WALKING, PLAYER_JUMPING, PLAYER_HOP, PLAYER_FALLING, PLAYER_DEAD};

class Player : public GBAEngine::Component {
    public:
        Player();
        GBAEngine::Rect getCollider();

        static Player* getSingleton();
        static int playerHealth;
        static int playerMaxHealth;
        static int playerMaxHealthProgress;
        GBAEngine::Rect collider;
        GBAEngine::Rect resizedCollider;
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
        static Player* singleton;
        void checkForEnemyContact(Enemy *enemy);

        
};

#endif