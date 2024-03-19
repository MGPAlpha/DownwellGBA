#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"

const extern GameObjectType playerType;

extern int playerHealth;
extern int playerMaxHealth;
extern int playerMaxHealthProgress;

extern GameObject *playerSingleton;

enum PLAYERSTATE {PLAYER_IDLE, PLAYER_WALKING, PLAYER_JUMPING, PLAYER_HOP, PLAYER_FALLING, PLAYER_DEAD};

typedef struct playerdata {
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
} PlayerData;

#endif