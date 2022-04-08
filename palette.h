#ifndef PALETTE_H
#define PALETTE_H

typedef struct palette {
    unsigned short colors[4];
} Palette;

void loadPalette(int index);

int getCurrPalette(void);

#endif