#ifndef COLLISION_H
#define COLLISION_H

#include "HW05Lib.hpp"
#include "engine/gbamath.hpp"

extern char *activeCollisionMap;
extern char activeCollisionMapWidth;

// To be returned by collision detection functions
// collided represents boolean of whether a collision was detected
// push represents adjustment necessary to move out of collision
typedef struct Collision {
    int collided;
    GBAEngine::Vector2 push;
} Collision;

// Check if two rects are colliding, the '.push' returned represents adjustment to rect1, not rect2
Collision collideRects(GBAEngine::Rect rect1, GBAEngine::Rect rect2);

// Check if obj is fully inside the bounds
Collision collideRectBounds(GBAEngine::Rect obj, GBAEngine::Rect bounds);

Collision collideCollisionMap(GBAEngine::Rect obj, char* map, int mapWidth, int relativeMagnitude);

#endif