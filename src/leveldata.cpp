#include "leveldata.hpp"

EnemySpawn emptyEnemies[0];

const char levelStart1Terrain[] = {
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,1,0,0,1,1,1,0,0,1,1,1,
    1,0,0,0,1,1,1,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1
};

const char levelEnd1Terrain[] = {
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,1,1,0,0,0,0,0,1,1,1,1,
    1,1,1,1,0,0,0,1,1,1,1,1,
    1,1,1,1,0,0,0,1,1,1,1,1,
    1,1,1,1,0,0,0,1,1,1,1,1,
    1,1,1,1,0,0,0,1,1,1,1,1,
    1,1,1,1,0,0,0,1,1,1,1,1,
    1,1,1,1,0,0,0,1,1,1,1,1
};

const char levelCavern1Terrain[] = {
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,1,1,0,0,0,0,0,0,0,1,1,
    1,1,1,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,1,1,1,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1
};

const EnemySpawn levelCavern1Enemies[] = {
    &blobType, {8, 2},
    &blobType, {2, 9}
};

const char levelCavern2Terrain[] = {
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,1,1,0,0,0,0,0,0,0,1,1,
    1,1,0,0,0,0,0,1,1,0,1,1,
    1,0,0,0,0,0,0,1,1,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1
};

const EnemySpawn levelCavern2Enemies[] = {
    &blobType, {8, 2}
};

const char levelCavern3Terrain[] = {
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,1,1,0,0,0,0,0,0,0,1,1,
    1,1,1,0,0,0,0,0,0,0,1,1,
    1,1,1,1,0,0,0,0,0,0,1,1,
    1,1,1,1,0,0,0,0,0,1,1,1,
    1,1,1,1,0,0,0,0,1,1,1,1,
    1,1,1,1,0,0,0,0,1,1,1,1,
    1,1,0,0,0,0,0,0,1,1,1,1,
    1,0,0,0,0,0,0,0,1,1,1,1,
    1,0,0,0,0,0,0,0,0,1,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,0,0,0,1,1
};

const EnemySpawn levelCavern3Enemies[] = {
    &blobType, {5, 5},
    &blobType, {2, 9}
};

const LevelSegment startSegments[] = {
    CREATE_LEVEL_SEGMENT(emptyEnemies, levelStart1Terrain)
};

const SegmentPool startSegmentPool = {startSegments, sizeof startSegments / sizeof(LevelSegment)};

const LevelSegment cavernSegments[] = {
    CREATE_LEVEL_SEGMENT(levelCavern1Enemies,levelCavern1Terrain),
    CREATE_LEVEL_SEGMENT(levelCavern2Enemies,levelCavern2Terrain),
    CREATE_LEVEL_SEGMENT(levelCavern3Enemies,levelCavern3Terrain)
};

const SegmentPool cavernSegmentPool = {cavernSegments, sizeof cavernSegments / sizeof(LevelSegment)};

const LevelSegment endSegments[] = {
    CREATE_LEVEL_SEGMENT(emptyEnemies, levelEnd1Terrain)
};

const SegmentPool endSegmentPool = {endSegments, sizeof endSegments / sizeof(LevelSegment)};