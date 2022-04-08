#ifndef OVERLAY_H
#define OVERLAY_H

#include "menus.h"

void initOverlay(void);

void clearOverlayCenter(void);

void fillOverlayCenter(void);

void fillOverlayRect(int col, int row, int width, int height, short tileId);

void printToOverlay(char *str, int col, int row, int font);

extern unsigned int currentMenuIndex;

void loadMenu(Menu* menu);

void updateMenu(void);

void drawMenu(Menu *menu, int selectedIndex, int col, int row, int width, int height);

void drawCurrentMenu(void);

#endif