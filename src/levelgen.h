#ifndef LEVELGEN_H
#define LEVELGEN_H

#include "leveldata.h"

#include "player.h"

extern int currentLevelLength;

void generateLevel(const SegmentPool *startPool, const SegmentPool *mainPool, const SegmentPool *endPool);

void generateTilemapUntil(int row);

void spawnNecessaryEnemies(PlayerData* playerData);

#endif