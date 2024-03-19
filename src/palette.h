#ifndef PALETTE_H
#define PALETTE_H

typedef struct palette {
    char* name;
    unsigned short colors[4];
} Palette;

extern const Palette palettes[];

void loadPalette(int index);

int getCurrPalette(void);

void initPalette(void);

void unlockPalette(int index);

void unlockAllPalettes(void);

#endif