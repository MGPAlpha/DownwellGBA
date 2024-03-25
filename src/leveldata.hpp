#ifndef LEVELDATA_HPP
#define LEVELDATA_HPP

#include "enemy.hpp"

#define LEVEL_WIDTH 12
#define MAX_LEVEL_LENGTH 400

typedef struct enemyspawn {
    const GameObjectGenerator *type;
    Vector2 pos;
} EnemySpawn;

typedef struct levelsegment {
    const EnemySpawn *enemies;
    int enemyCount;
    const char *terrainData;
    int terrainDataLines;
} LevelSegment;

typedef struct segmentpool {
    const LevelSegment *pool;
    int poolSize;
} SegmentPool;

#define CREATE_LEVEL_SEGMENT(enemies,terrain) (LevelSegment){(enemies),sizeof(enemies)/sizeof(EnemySpawn),(terrain),sizeof(terrain)/sizeof(char)/LEVEL_WIDTH}

extern const SegmentPool startSegmentPool;
extern const SegmentPool cavernSegmentPool;
extern const SegmentPool endSegmentPool;

#endif