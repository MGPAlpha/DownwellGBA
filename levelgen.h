#ifndef LEVELGEN_H
#define LEVELGEN_H

#include "leveldata.h"

#include "player.h"

void generateLevel(SegmentPool *startPool, SegmentPool *mainPool);

void generateTilemapUntil(int row);

void spawnNecessaryEnemies(PlayerData* playerData);

#endif