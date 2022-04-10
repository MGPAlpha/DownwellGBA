#ifndef CHEATS_H
#define CHEATS_H

#define NUM_CHEATS 10

void initCheats(void);

typedef struct cheat {
    char name[16];
    void (*invoke)(void);
    void (*toggle)(void);
    void (*disable)(void);
    int *enabled;
} Cheat;

extern Cheat cheats[];

#endif