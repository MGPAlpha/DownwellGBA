#ifndef COLLISION_H
#define COLLISION_H

#include "HW05Lib.h"

// To be returned by collision detection functions
// collided represents boolean of whether a collision was detected
// push represents adjustment necessary to move out of collision
typedef struct Collision {
    int collided;
    Vector2 push;
} Collision;

// Check if two rects are colliding, the '.push' returned represents adjustment to rect1, not rect2
Collision collideRects(Rect rect1, Rect rect2);

// Check if obj is fully inside the bounds
Collision collideRectBounds(Rect obj, Rect bounds);

#endif