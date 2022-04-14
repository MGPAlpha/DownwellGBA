#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"

const extern GameObjectType playerType;

extern GameObject *playerSingleton;

enum PLAYERSTATE {PLAYER_IDLE, PLAYER_WALKING, PLAYER_JUMPING, PLAYER_HOP, PLAYER_FALLING};

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
} PlayerData;

#endif