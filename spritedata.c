#include "spritedata.h"
#include "stdlib.h"

static OBJ_ATTR shadowOAM[128];

void hideSprites(void) {
    for (int i = 0; i < 128; i++) {
        shadowOAM[i].attr0 = ATTR0_HIDE;
    }
    waitForVBlank();
    DMANow(3, shadowOAM, OAM, 128*sizeof(OBJ_ATTR)/2);
}

static OBJ_ATTR *dynamicOAM[128-RESERVED_OAM_SIZE];

static int dynamicOAMIndex = 0;

void initSpriteMemory(void) {
    for (int i = RESERVED_OAM_SIZE; i < 128; i++) {
        dynamicOAM[i - RESERVED_OAM_SIZE] = shadowOAM + i;
    }
    dynamicOAMIndex = 0;
}

OBJ_ATTR *spriteAlloc(void) {
    if (dynamicOAMIndex < sizeof dynamicOAM / sizeof(OBJ_ATTR)) {
        return dynamicOAM[dynamicOAMIndex++];
    }
    return NULL;
}

void spriteFree(OBJ_ATTR* sprite) {
    dynamicOAM[--dynamicOAMIndex] = sprite;
}

void updateSprites(void) {
    DMANow(3, shadowOAM, OAM, 128*sizeof(OBJ_ATTR)/2);
}