#ifndef GEM_HPP
#define GEM_HPP

#include "gameobject.hpp"

const extern GameObjectType gemType;

enum GEMSTATE {GEM_PHYSICS, GEM_ATTRACT};
enum GEMTYPE {GEM_SMALL, GEM_LARGE};
enum GEMROTATION {GEM_CW, GEM_CCW};

typedef struct gemdata {
    Rect collider;
    Vector2 velocity;
    enum GEMSTATE state;
    int stateTime;
    enum GEMTYPE type;
    enum GEMROTATION rotationDirection;
} GemData;

extern int gemsHeld;
extern int totalGemsThisRun;
extern int lifetimeGems;

void initGemData(void);

void collectGems(int gems);

GameObject *spawnGem(Vector2 pos);

void randomizeGem(GameObject *self);

#endif