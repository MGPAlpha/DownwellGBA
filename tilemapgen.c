#include "tilemapgen.h"
#include "collision.h"

int furthestGenerated = 0;

void resetTilemapGen(void) {
    furthestGenerated = 0;
}

void generateTilemapUntil(int row) {
    for (int i = furthestGenerated+1; i <= row; i++) {
        for (int j = 0; j < activeCollisionMapWidth; j++) {
            short largeTileIndex = 0;
            if (activeCollisionMap[i*activeCollisionMapWidth+j]) {
                largeTileIndex = (j+1 >= activeCollisionMapWidth || activeCollisionMap[i*activeCollisionMapWidth+j+1] ? 1 : 0) +
                                (i-1 < 0 || activeCollisionMap[(i-1)*activeCollisionMapWidth+j] ? 2 : 0) +
                                (j-1 < 0 || activeCollisionMap[i*activeCollisionMapWidth+j-1] ? 4 : 0) +
                                (activeCollisionMap[(i+1)*activeCollisionMapWidth+j] ? 8 : 0);
                largeTileIndex += 16;
            }

            short tileIndex = largeTileIndex/16*64 + largeTileIndex%16*2 | 1<<12;

            int tilemapOffset = OFFSET(j*2,i*2,32)%(32*32);
            if (j >= 16) tilemapOffset += 32*32;
            if (i % 32 >= 16) tilemapOffset += 32*32*2;

            SCREENBLOCK[24].tilemap[tilemapOffset] = tileIndex;
            SCREENBLOCK[24].tilemap[tilemapOffset+1] = tileIndex+1;
            SCREENBLOCK[24].tilemap[tilemapOffset+32] = tileIndex+32;
            SCREENBLOCK[24].tilemap[tilemapOffset+33] = tileIndex+33;
        }
    }

    furthestGenerated = row;
}