#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "gameobject.hpp"

// const extern GameObjectType playerType;

enum PLAYERSTATE {PLAYER_IDLE, PLAYER_WALKING, PLAYER_JUMPING, PLAYER_HOP, PLAYER_FALLING, PLAYER_DEAD};

class Player : Component {
    public:
        Player();
    protected:
        void awake() override;
        void update() override;
        void draw() override;
        void destroy() override;

        static Player* getSingleton();
    private:
        static Player* singleton;
        static int playerHealth;
        static int playerMaxHealth;
        static int playerMaxHealthProgress;
        
        Rect collider;
        Rect resizedCollider;
        enum PLAYERSTATE state;
        int stateTime;
        enum DIRECTION dir;
        int runningJump;
        int canFire;
        int fireTime;
        int charge;
        int ammo;
        int iFrames;
};

#endif