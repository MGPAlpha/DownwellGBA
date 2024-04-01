#include "collision.hpp"

#include <cstdlib>

extern "C" {
    #include "print.h"
}

using namespace GBAEngine;

char *activeCollisionMap = NULL;
char activeCollisionMapWidth = 0;

// Returns a collision struct, with the push property representing how much to move rect1 to move it out of the collision
Collision collideRects(Rect rect1, Rect rect2) {
    Collision out = {0,{0,0}};
    
    fixed32 x12 = rect2.pos.x - (rect1.pos.x + rect1.size.x);
    fixed32 x21 = rect1.pos.x - (rect2.pos.x + rect2.size.x);
    fixed32 y12 = rect2.pos.y - (rect1.pos.y + rect1.size.y);
    fixed32 y21 = rect1.pos.y - (rect2.pos.y + rect2.size.y);

    out.collided = x12 < 0 && x21 < 0 && y12 < 0 && y21 < 0;

    if (out.collided) {
        fixed32 closestSideX = x21 > x12 ? -1 : 1;
        fixed32 closestSideY = y21 > y12 ? -1 : 1;
        fixed32 closestDistX = x21 > x12 ? x21 : x12;
        fixed32 closestDistY = y21 > y12 ? y21 : y12;
        
        if (x12 < 0) out.push.x -= x12; 
        if (x21 < 0) out.push.x += x21;
        if (y12 < 0) out.push.y -= y12; 
        if (y21 < 0) out.push.y += y21; 
    }
    return out;
}

Collision collideRectBounds(Rect obj, Rect bounds) {
    Collision out;
    out.push.x = obj.pos.x < bounds.pos.x ? bounds.pos.x - obj.pos.x : min(bounds.pos.x + bounds.size.x - (obj.pos.x + obj.size.x),0);
    out.push.y = obj.pos.y < bounds.pos.y ? bounds.pos.y - obj.pos.y : min(bounds.pos.y + bounds.size.y - (obj.pos.y + obj.size.y),0);
    out.collided = out.push.x || out.push.y;
    return out;
}

Collision collideCollisionMap(Rect obj, char* map, int mapWidth, int relativeMagnitude) {
    Collision out = {0, {0,0}};

    Vector2 upperLeftCorner = obj.pos;
    Vector2 upperRightCorner = {obj.pos.x+obj.size.x-1, obj.pos.y};
    Vector2 lowerLeftCorner = {obj.pos.x, obj.pos.y+obj.size.y-1};
    Vector2 lowerRightCorner = {obj.pos.x+obj.size.x-1, obj.pos.y+obj.size.y-1};

    Vector2 upperLeftReduced = {upperLeftCorner.x.value >> relativeMagnitude, upperLeftCorner.y.value >> relativeMagnitude};
    Vector2 upperRightReduced = {upperRightCorner.x.value >> relativeMagnitude, upperRightCorner.y.value >> relativeMagnitude};
    Vector2 lowerLeftReduced = {lowerLeftCorner.x.value >> relativeMagnitude, lowerLeftCorner.y.value >> relativeMagnitude};
    Vector2 lowerRightReduced = {lowerRightCorner.x.value >> relativeMagnitude, lowerRightCorner.y.value >> relativeMagnitude};

    char upperLeftCollision = map[OFFSET(int(upperLeftReduced.x), int(upperLeftReduced.y), mapWidth)];
    char upperRightCollision = map[OFFSET(int(upperRightReduced.x), int(upperRightReduced.y), mapWidth)];
    char lowerLeftCollision = map[OFFSET(int(lowerLeftReduced.x), int(lowerLeftReduced.y), mapWidth)];
    char lowerRightCollision = map[OFFSET(int(lowerRightReduced.x), int(lowerRightReduced.y), mapWidth)];
    
    int leftPush = 0;
    int rightPush = 0;
    int upPush = 0;
    int downPush = 0;

    if (upperLeftCollision) {
        rightPush = max(rightPush, (int(upperLeftReduced.x+1)<<relativeMagnitude)-upperLeftCorner.x.value);
        downPush = max(downPush, (int(upperLeftReduced.y+1)<<relativeMagnitude)-upperLeftCorner.y.value);
    }
    if (upperRightCollision) {
        leftPush = max(leftPush, upperRightCorner.x.value-(int(upperRightReduced.x)<<relativeMagnitude)+1);
        downPush = max(downPush, (int(upperRightReduced.y+1)<<relativeMagnitude)-upperRightCorner.y.value);
    }
    if (lowerLeftCollision) {
        rightPush = max(rightPush, (int(lowerLeftReduced.x+1)<<relativeMagnitude)-lowerLeftCorner.x.value);
        upPush = max(upPush, lowerLeftCorner.y.value-(int(lowerLeftReduced.y)<<relativeMagnitude)+1);
    }
    if (lowerRightCollision) {
        leftPush = max(leftPush, lowerRightCorner.x.value-(int(lowerRightReduced.x)<<relativeMagnitude)+1);
        upPush = max(upPush, lowerRightCorner.y.value-(int(lowerRightReduced.y)<<relativeMagnitude)+1);
    }

    out.collided = (upperLeftCollision || upperRightCollision || lowerLeftCollision || lowerRightCollision);
    out.push.x.value = rightPush - leftPush;
    out.push.y.value = downPush - upPush;

    return out;
}