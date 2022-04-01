#include "mgpmath.h"
#include "stdlib.h"

int randRange(int min, int max) {
    int randVal = (int)( rand() % (max-min) );
    return randVal + min;
}

int sign(int v) {
    return (v > 0) ? 1 : (v < 0 ? -1 : 0);
}