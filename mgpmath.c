#include "mgpmath.h"
#include "stdlib.h"

Rect resizeRect(Rect r, int magnitudeDifference) {
    r.pos.x <<= magnitudeDifference;
    r.pos.y <<= magnitudeDifference;
    r.size.x <<= magnitudeDifference;
    r.size.y <<= magnitudeDifference;
    return r;
}

int randRange(int min, int max) {
    int randVal = (int)( rand() % (max-min) );
    return randVal + min;
}

int sign(int v) {
    return (v > 0) ? 1 : (v < 0 ? -1 : 0);
}