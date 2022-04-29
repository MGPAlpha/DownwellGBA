#include "unlocks.h"

Unlock unlocks[] = {
    {.quota = 1000, .index = 1},
    {.quota = 2000, .index = 2},
    {.quota = 3000, .index = 3},
    {.quota = 4000, .index = 4},
    {.quota = 5000, .index = 5},
    {.quota = 7500, .index = 6},
    {.quota = 10000, .index = 7},
    {.quota = 12500, .index = 8},
    {.quota = 15000, .index = 9},
    {.quota = 17500, .index = 10},
    {.quota = 20000, .index = 11},
    {.quota = 25000, .index = 12},
    {.quota = 30000, .index = 13},
    {.quota = 35000, .index = 14},
    {.quota = 40000, .index = 15}
};

const int unlockSize = sizeof(unlocks)/sizeof(Unlock);

int getNextUnlockIndex(int gems) {
    int i = 0;
    for (; i < sizeof unlocks / sizeof(Unlock); i++) {
        if (unlocks[i].quota > gems) return i;
    }
    return i-1;
}