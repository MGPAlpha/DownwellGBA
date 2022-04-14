#include "overlay.h"

#include "HW05Lib.h"
#include "menus.h"
#include "stdlib.h"

#include "art/overlaytiles.h"

void initOverlay(void) {


    REG_BG1CNT = BG_CHARBLOCK(2) | BG_SCREENBLOCK(31) | BG_4BPP | BG_SIZE_SMALL | 0; // UI Overlay

    // DMANow(3, overlaytiles_palette, PALETTE+16, OVERLAYTILES_PALETTE_LENGTH);
    DMANow(3, overlaytiles, &CHARBLOCK[2], OVERLAYTILES_LENGTH);
    short blackTile = (32*9+1) | 1<<12;
    DMANow(3, &blackTile, SCREENBLOCK[31].tilemap, DMA_SOURCE_FIXED | 1024);

    SCREENBLOCK[31].tilemap[4] = (32*9+3) | 1<<12;
    SCREENBLOCK[31].tilemap[4+21] = (32*9+3) | 1<<12 | 1<<10;
    SCREENBLOCK[31].tilemap[4+19*32] = (32*9+3) | 1<<12 | 1<<11;
    SCREENBLOCK[31].tilemap[4+19*32+21] = (32*9+3) | 1<<12 | 1<<10 | 1<<11;
    for (int i = 1; i < 19; i++) {
        SCREENBLOCK[31].tilemap[4+i*32] = (32*9+2) | 1<<12;
    }
    for (int i = 1; i < 19; i++) {
        SCREENBLOCK[31].tilemap[4+21+i*32] = (32*9+2) | 1<<12 | 1<<10;
    }

    clearOverlayCenter();

    SCREENBLOCK[31].tilemap[26+32] = (32*9+4) | 1<<12;
    SCREENBLOCK[31].tilemap[27+32] = (32*9+5) | 1<<12;
    SCREENBLOCK[31].tilemap[28+32] = (32*9+6) | 1<<12;
    SCREENBLOCK[31].tilemap[29+32] = (32*9+7) | 1<<12;
    SCREENBLOCK[31].tilemap[28+32*2] = (32*10+6) | 1<<12;
    SCREENBLOCK[31].tilemap[29+32*2] = (32*10+7) | 1<<12;

    SCREENBLOCK[31].tilemap[OFFSET(26,2,32)] = OFFSET(0, 10, 32) | 1 << 12;
    SCREENBLOCK[31].tilemap[OFFSET(27,2,32)] = OFFSET(0, 10, 32) | 1 << 12 | 1<<10;
    SCREENBLOCK[31].tilemap[OFFSET(26,17,32)] = OFFSET(0, 10, 32) | 1 << 12 | 1<<11;
    SCREENBLOCK[31].tilemap[OFFSET(27,17,32)] = OFFSET(0, 10, 32) | 1 << 12 | 1<<10 | 1<<11;

    updateHealthDisplay(4,12,2);

    printToOverlay("0000", 26, 0, 0);

    printToOverlay("DOWN", 0, 17, 0);
    printToOverlay("WELL", 0, 18, 1);
    printToOverlay("GBA", 0, 19, 2);

    updateAmmoDisplay(0,1);
    
}

void clearOverlayCenter(void) {
    short clearTile = (32*9) | 1<<12;
    for (int i = 0; i < 32; i++) {
        DMANow(3, &clearTile, SCREENBLOCK[31].tilemap + 32 * i + 5, DMA_SOURCE_FIXED | 20);
    }
}

void fillOverlayCenter(void) {
    short fillTile = (32*9+1) | 1<<12;
    for (int i = 0; i < 32; i++) {
        DMANow(3, &fillTile, SCREENBLOCK[31].tilemap + 32 * i + 5, DMA_SOURCE_FIXED | 20);
    }
}

void fillOverlayRect(int col, int row, int width, int height, short tileId) {
    short fillTile = tileId | 1<<12;
    for (int i = row; i < height; i++) {
        DMANow(3, &fillTile, SCREENBLOCK[31].tilemap + 32 * i + col, DMA_SOURCE_FIXED | width);
    }
}

void printToOverlay(char *str, int col, int row, int font) {
    while(*str) {
        SCREENBLOCK[31].tilemap[OFFSET(col, row, 32)] = *str + 3*32 * font - 32 | 1<<12;
        str++;
        col++;
    }
}

void updateAmmoDisplay(int ammo, int perShot) {
    for (int i = 0; i < 14; i++) {
        int tileCol = 16-i;
        int tileIndex = OFFSET(ammo - 4 * i - 1, 10+perShot,32);
        if (ammo <= i*4) tileIndex = OFFSET(2,10,32); // Fully Empty tile
        if (ammo - perShot >= 4*(i+1)) tileIndex = OFFSET(1,10,32); // Filled tile
        SCREENBLOCK[31].tilemap[OFFSET(26,tileCol,32)] = tileIndex | 1<<12;
        SCREENBLOCK[31].tilemap[OFFSET(27,tileCol,32)] = tileIndex | 1<<12 | 1<<10;
    }
    char ammoText[4];
    sprintf(ammoText, "%d", ammo);
    printToOverlay(ammoText, 26, 18, 0);
}

void updateHealthDisplay(int health, int max, int maxProgress) {
    SCREENBLOCK[31].tilemap[OFFSET(0,0,32)] = OFFSET(8,9,32) | 1<<12;
    SCREENBLOCK[31].tilemap[OFFSET(1,0,32)] = OFFSET(9,9,32) | 1<<12;
    SCREENBLOCK[31].tilemap[OFFSET(2,0,32)] = OFFSET(9,9,32) | 1<<12;
    SCREENBLOCK[31].tilemap[OFFSET(3,0,32)] = OFFSET(8,9,32) | 1<<10 | 1<<12;

    for (int i = 0; i < 4; i++) {
        short tileIndex = health * 32 / max - i * 8;
        if (tileIndex < 0) tileIndex = 0;
        if (tileIndex > 8) tileIndex = 8;
        SCREENBLOCK[31].tilemap[OFFSET(i,1,32)] = OFFSET(8+tileIndex,10,32) | 1<<12;
    }

    for (int i = 0; i < 4; i++) {
        short tileIndex = 0;
        if (maxProgress > i) tileIndex += 2;
        if (i == 1 || i == 2) tileIndex += 1;
        SCREENBLOCK[31].tilemap[OFFSET(i,2,32)] = OFFSET(8+tileIndex,11,32) | (i==3?1:0)<<10 | 1<<12;
    }

    char currHealthString[4];
    sprintf(currHealthString, "%d", health);
    char maxHealthString[4];
    sprintf(maxHealthString, "%d", max);

    printToOverlay(currHealthString, 2, 4, 2);
    printToOverlay(maxHealthString, 2, 6, 0);
    SCREENBLOCK[31].tilemap[OFFSET(0,4,32)] = OFFSET(11,9,32) | 1<<12;
    SCREENBLOCK[31].tilemap[OFFSET(2,5,32)] = OFFSET(10,9,32) | 1<<12;
    SCREENBLOCK[31].tilemap[OFFSET(3,5,32)] = OFFSET(10,9,32) | 1<<10 | 1<<11 | 1<<12;
}

Menu *currentMenu = NULL;
unsigned int currentMenuIndex = 0;

void loadMenu(Menu* menu) {
    currentMenu = menu;
    currentMenuIndex = 0;
    if (currentMenu->onLoad) {
        (*currentMenu->onLoad)();
    }
}

void updateMenu(void) {
    int indexChange = 0;
    if (BUTTON_PRESSED(BUTTON_LEFT) || BUTTON_PRESSED(BUTTON_UP)) {
        indexChange += -1;
    }
    if (BUTTON_PRESSED(BUTTON_RIGHT) || BUTTON_PRESSED(BUTTON_DOWN)) {
        indexChange += 1;
    }
    if (indexChange) {
        int initialIndex = currentMenuIndex;
        do {
            currentMenuIndex += indexChange;
            currentMenuIndex += currentMenu->itemCount;
            currentMenuIndex %= currentMenu->itemCount;
        } while (currentMenuIndex != initialIndex
            && currentMenu->items[currentMenuIndex].shouldAppear
            && !(*currentMenu->items[currentMenuIndex].shouldAppear)());
    } else if (BUTTON_PRESSED(BUTTON_A) || BUTTON_PRESSED(BUTTON_B)) {
        
            MenuItem *selectedItem = currentMenu->items + currentMenuIndex;
            if (selectedItem->behaviorMode == MENU_SUBMENU_BEHAVIOR) {
                if (selectedItem->behavior.submenu) loadMenu(selectedItem->behavior.submenu);
            } else {
                if (selectedItem->behavior.func) (*selectedItem->behavior.func)(currentMenuIndex);
            }
    }
    if (currentMenu->onSelect) (*currentMenu->onSelect)(currentMenuIndex);
}

void drawMenu(Menu *menu, int selectedIndex, int col, int row, int width, int height) {
    for (int i = 0, screenIndex = 0; i < height && i < menu->itemCount; i++, screenIndex++) {
        MenuItem *currItem = menu->items + i;
        if (currItem->shouldAppear && !(*currItem->shouldAppear)()) {
            screenIndex--;
            continue;
        }
        if (currItem->textMode == MENU_FUNCTION_TEXT) {
            char textOut[20];
            (*currItem->itemText.func)(textOut, i);
            printToOverlay(textOut, col + screenIndex, row + screenIndex, i == selectedIndex ? 2 : 1);
        } else {
            printToOverlay(currItem->itemText.text, col + screenIndex, row + screenIndex, i == selectedIndex ? 2 : 1);
        }
    }
}

void drawScrollingMenu(Menu *menu, int selectedIndex, int col, int row, int width, int height) {
    int indexOffset = -(height/2);
    for (int i = 0; i < height; i++) {
        int itemIndex = mod((i + indexOffset + selectedIndex), menu->itemCount);
        MenuItem *currItem = menu->items + itemIndex;
        if (currItem->textMode == MENU_FUNCTION_TEXT) {
            
        } else {
            printToOverlay(currItem->itemText.text, col + i, row + i, i == height/2 ? 2 : 1);
        }
    }
}

void drawCurrentMenu(void) {
    if (currentMenu) {
        (*currentMenu->draw)(currentMenu, currentMenuIndex);
    }
}