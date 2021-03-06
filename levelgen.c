#include "levelgen.h"
#include "HW05Lib.h"
#include "collision.h"

EnemySpawn levelEnemies[MAX_LEVEL_LENGTH];
int levelEnemyCount = 0;

char gameCollision[MAX_LEVEL_LENGTH * LEVEL_WIDTH];

int currentLevelLength = 0;
int furthestGenerated = 0;
int enemySpawnIndex = 0;

void addSegmentToLevel(LevelSegment *seg) {
    for (int i = 0; i < seg->enemyCount; i++) {
        levelEnemies[levelEnemyCount] = seg->enemies[i];
        levelEnemies[levelEnemyCount].pos.y += currentLevelLength;
        levelEnemyCount++;
    }

    DMANow(3, seg->terrainData, gameCollision + LEVEL_WIDTH * currentLevelLength, LEVEL_WIDTH * seg->terrainDataLines / 2);
    currentLevelLength += seg->terrainDataLines;
}

void generateLevel(SegmentPool *startPool, SegmentPool *mainPool, SegmentPool *endPool) {
    currentLevelLength = 0;
    furthestGenerated = 0;
    levelEnemyCount = 0;
    enemySpawnIndex = 0;

    activeCollisionMap = gameCollision;
    activeCollisionMapWidth = LEVEL_WIDTH;

    int startSegIndex = randRange(0, startPool->poolSize);
    LevelSegment *startSeg = startPool->pool + startSegIndex;
    addSegmentToLevel(startSeg);

    int endSegIndex = randRange(0, endPool->poolSize);
    LevelSegment *endSeg = endPool->pool + endSegIndex;

    int consecutiveBadSegments = 0;
    while (currentLevelLength < MAX_LEVEL_LENGTH && consecutiveBadSegments < 3) {
        LevelSegment *newSeg = mainPool->pool + randRange(0, mainPool->poolSize);
        if (newSeg->terrainDataLines > MAX_LEVEL_LENGTH - endSeg->terrainDataLines - currentLevelLength) {
            consecutiveBadSegments++;
        } else {
            consecutiveBadSegments = 0;
            addSegmentToLevel(newSeg);
        }
    }

    addSegmentToLevel(endSeg);
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

void spawnNecessaryEnemies(PlayerData *playerData) {
    while (enemySpawnIndex < levelEnemyCount && levelEnemies[enemySpawnIndex].pos.y < (playerData->collider.pos.y>>4) + 16) {
        EnemySpawn spawn = levelEnemies[enemySpawnIndex];
        Vector2 spawnPos = spawn.pos;
        spawnPos.x <<= 4;
        spawnPos.y <<= 4;
        spawnPos.x += 8 - spawn.type->colliderSize.x / 2;
        spawnPos.y += 8 - spawn.type->colliderSize.y / 2;
        GameObject *newEnemy = spawnEnemy(spawn.type, spawnPos);
        if (!newEnemy) mgba_printf("failed to spawn enemy %d", enemySpawnIndex);
        enemySpawnIndex++;
    }
}