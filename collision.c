#include "collision.h"

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