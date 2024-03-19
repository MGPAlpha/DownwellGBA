#ifndef UNLOCKS_H
#define UNLOCKS_h

typedef struct unlock {
    int index;
    int quota;
} Unlock;

extern Unlock unlocks[];

extern const int unlockSize;

int getNextUnlockIndex(int gems);

#endif