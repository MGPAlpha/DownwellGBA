#ifndef LEVELDATA_H
#define LEVELDATA_H

#include "enemy.h"

#define LEVEL_WIDTH 12
#define MAX_LEVEL_LENGTH 400

typedef struct enemyspawn {
    EnemyType *type;
    Vector2 pos;
} EnemySpawn;

typedef struct levelsegment {
    EnemySpawn *enemies;
    int enemyCount;
    char *terrainData;
    int terrainDataLines;
} LevelSegment;

typedef struct segmentpool {
    LevelSegment *pool;
    int poolSize;
} SegmentPool;

#define CREATE_LEVEL_SEGMENT(enemies,terrain) (LevelSegment){(enemies),sizeof(enemies)/sizeof(EnemySpawn),(terrain),sizeof(terrain)/sizeof(char)/LEVEL_WIDTH}

extern const SegmentPool startSegmentPool;
extern const SegmentPool cavernSegmentPool;
extern const SegmentPool endSegmentPool;

#endif