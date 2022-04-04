#include "collision.h"
#include "stdlib.h"

char *activeCollisionMap = NULL;
char activeCollisionMapWidth = 0;

// Returns a collision struct, with the push property representing how much to move rect1 to move it out of the collision
Collision collideRects(Rect rect1, Rect rect2) {
    Collision out = {0,{0,0}};
    
    int x12 = rect2.pos.x - (rect1.pos.x + rect1.size.x);
    int x21 = rect1.pos.x - (rect2.pos.x + rect2.size.x);
    int y12 = rect2.pos.y - (rect1.pos.y + rect1.size.y);
    int y21 = rect1.pos.y - (rect2.pos.y + rect2.size.y);

    out.collided = x12 < 0 && x21 < 0 && y12 < 0 && y21 < 0;

    if (out.collided) {
        int closestSide = 0;
        int closestDist = x12;
        if (x21>closestDist) {
            closestSide = 1;
            closestDist = x21;
        }
        if (y12>closestDist) {
            closestSide = 2;
            closestDist = y12;
        }
        if (y21>closestDist) {
            closestSide = 3;
            closestDist = y21;
        }
        switch (closestSide) {
            case 0:
                out.push.x = closestDist;
                break;
            case 1:
                out.push.x = -closestDist;
                break;
            case 2:
                out.push.y = closestDist;
                break;
            case 3:
                out.push.y = -closestDist;
                break;
        }
    }
    return out;
}

Collision collideRectBounds(Rect obj, Rect bounds) {
    Collision out;
    out.push.x = obj.pos.x < bounds.pos.x ? bounds.pos.x - obj.pos.x : MIN(bounds.pos.x + bounds.size.x - (obj.pos.x + obj.size.x),0);
    out.push.y = obj.pos.y < bounds.pos.y ? bounds.pos.y - obj.pos.y : MIN(bounds.pos.y + bounds.size.y - (obj.pos.y + obj.size.y),0);
    out.collided = out.push.x || out.push.y;
    return out;
}

Collision collideCollisionMap(Rect obj, char* map, int mapWidth, int relativeMagnitude) {
    Collision out = {0, {0,0}};

    Vector2 upperLeftCorner = obj.pos;
    Vector2 upperRightCorner = {obj.pos.x+obj.size.x-1, obj.pos.y};
    Vector2 lowerLeftCorner = {obj.pos.x, obj.pos.y+obj.size.y-1};
    Vector2 lowerRightCorner = {obj.pos.x+obj.size.x-1, obj.pos.y+obj.size.y-1};

    Vector2 upperLeftReduced = {upperLeftCorner.x >> relativeMagnitude, upperLeftCorner.y >> relativeMagnitude};
    Vector2 upperRightReduced = {upperRightCorner.x >> relativeMagnitude, upperRightCorner.y >> relativeMagnitude};
    Vector2 lowerLeftReduced = {lowerLeftCorner.x >> relativeMagnitude, lowerLeftCorner.y >> relativeMagnitude};
    Vector2 lowerRightReduced = {lowerRightCorner.x >> relativeMagnitude, lowerRightCorner.y >> relativeMagnitude};

    char upperLeftCollision = map[OFFSET(upperLeftReduced.x, upperLeftReduced.y, mapWidth)];
    char upperRightCollision = map[OFFSET(upperRightReduced.x, upperRightReduced.y, mapWidth)];
    char lowerLeftCollision = map[OFFSET(lowerLeftReduced.x, lowerLeftReduced.y, mapWidth)];
    char lowerRightCollision = map[OFFSET(lowerRightReduced.x, lowerRightReduced.y, mapWidth)];
    
    int leftPush = 0;
    int rightPush = 0;
    int upPush = 0;
    int downPush = 0;

    if (upperLeftCollision) {
        rightPush = MAX(rightPush, ((upperLeftReduced.x+1)<<relativeMagnitude)-upperLeftCorner.x);
        downPush = MAX(downPush, ((upperLeftReduced.y+1)<<relativeMagnitude)-upperLeftCorner.y);
    }
    if (upperRightCollision) {
        leftPush = MAX(leftPush, upperRightCorner.x-(upperRightReduced.x<<relativeMagnitude)+1);
        downPush = MAX(downPush, ((upperRightReduced.y+1)<<relativeMagnitude)-upperRightCorner.y);
    }
    if (lowerLeftCollision) {
        rightPush = MAX(rightPush, ((lowerLeftReduced.x+1)<<relativeMagnitude)-lowerLeftCorner.x);
        upPush = MAX(upPush, lowerLeftCorner.y-(lowerLeftReduced.y<<relativeMagnitude)+1);
    }
    if (lowerRightCollision) {
        leftPush = MAX(leftPush, lowerRightCorner.x-(lowerRightReduced.x<<relativeMagnitude)+1);
        upPush = MAX(upPush, lowerRightCorner.y-(lowerRightReduced.y<<relativeMagnitude)+1);
    }

    out.collided = (upperLeftCollision || upperRightCollision || lowerLeftCollision || lowerRightCollision);
    out.push.x = rightPush - leftPush;
    out.push.y = downPush - upPush;

    return out;
}