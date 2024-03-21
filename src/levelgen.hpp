#ifndef LEVELGEN_HPP
#define LEVELGEN_HPP

#include "leveldata.hpp"

#include "player.hpp"

extern int currentLevelLength;

void generateLevel(const SegmentPool *startPool, const SegmentPool *mainPool, const SegmentPool *endPool);

void generateTilemapUntil(int row);

void spawnNecessaryEnemies(PlayerData* playerData);

#endif