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

    printToOverlay("0000", 26, 0, 0);

    printToOverlay("DOWN", 26, 17, 0);
    printToOverlay("WELL", 26, 18, 1);
    printToOverlay("GBA", 26, 19, 2);
    
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
    if (BUTTON_PRESSED(BUTTON_LEFT) || BUTTON_PRESSED(BUTTON_UP)) {
        currentMenuIndex--;
        currentMenuIndex+=currentMenu->itemCount;
        currentMenuIndex %= currentMenu->itemCount;
    } else if (BUTTON_PRESSED(BUTTON_RIGHT) || BUTTON_PRESSED(BUTTON_DOWN)) {
        currentMenuIndex++;
        currentMenuIndex %= currentMenu->itemCount;
    } else if (BUTTON_PRESSED(BUTTON_A) || BUTTON_PRESSED(BUTTON_B)) {
        
            MenuItem *selectedItem = currentMenu->items + currentMenuIndex;
            if (selectedItem->behaviorMode == MENU_SUBMENU_BEHAVIOR) {
                if (selectedItem->behavior.submenu) loadMenu(selectedItem->behavior.submenu);
            } else {
                if (selectedItem->behavior.func) (*selectedItem->behavior.func)();
            }
    }
    if (currentMenu->onSelect) (*currentMenu->onSelect)(currentMenuIndex);
}

void drawMenu(Menu *menu, int selectedIndex, int col, int row, int width, int height) {
    for (int i = 0; i < height && i < menu->itemCount; i++) {
        MenuItem *currItem = menu->items + i;
        if (currItem->textMode == MENU_FUNCTION_TEXT) {
            
        } else {
            printToOverlay(currItem->itemText.text, col + i, row + i, i == selectedIndex ? 2 : 1);
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