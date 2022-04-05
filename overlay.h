#ifndef OVERLAY_H
#define OVERLAY_H

void initOverlay(void);

void clearOverlayCenter(void);

void fillOverlayCenter(void);

void fillOverlayRect(int col, int row, int width, int height, short tileId);

void printToOverlay(char *str, int col, int row, int font);

#endif