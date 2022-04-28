#include "unlocks.h"

Unlock unlocks[] = {
    {.quota = 100, .index = 1},
    {.quota = 200, .index = 2},
    {.quota = 300, .index = 3}
};

const int unlockSize = sizeof(unlocks)/sizeof(Unlock);

int getNextUnlockIndex(int gems) {
    int i = 0;
    for (; i < sizeof unlocks / sizeof(Unlock); i++) {
        if (unlocks[i].quota > gems) return i;
    }
    return i-1;
}