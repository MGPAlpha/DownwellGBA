#include "gbamath.hpp"

#include <cstdlib>
#include <cmath>

#pragma region fixed32

fixed32 fixed32::sin(int theta) {
    if (!trigDataInitialized) {
        initializeTrigData();
    }
    theta %= 360;
    return sinData[theta];
}

fixed32 fixed32::cos(int theta) {
    if (!trigDataInitialized) {
        initializeTrigData();
    }
    theta += 90;
    theta %= 360;
    return sinData[theta];
}

fixed32 fixed32::tan(int theta) {
    if (!trigDataInitialized) {
        initializeTrigData();
    }
    theta %= 180;
    return tanData[theta];
}

bool fixed32::trigDataInitialized = false;

void fixed32::initializeTrigData() {
    initializeSin();
    initializeTan();
    trigDataInitialized = true;
}

fixed32 fixed32::sinData[360] = {};
fixed32 fixed32::tanData[180] = {};

void fixed32::initializeSin() {
    for (int i = 0; i < 360; i++) {
        sinData[i] = sinf(i * M_PI * 2 / 360.);
    }
}

void fixed32::initializeTan() {
    for (int i = 0; i < 180; i++) {
        tanData[i] = tanf(i * M_PI * 2 / 360.);
    }
}

#pragma endregion

Rect resizeRect(Rect r, int magnitudeDifference) {
    r.pos.x.value <<= magnitudeDifference;
    r.pos.y.value <<= magnitudeDifference;
    r.size.x.value <<= magnitudeDifference;
    r.size.y.value <<= magnitudeDifference;
    return r;
}

int randRange(int min, int max) {
    int randVal = (int)( rand() % (max-min) );
    return randVal + min;
}

int sign(int v) {
    return (v > 0) ? 1 : (v < 0 ? -1 : 0);
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}