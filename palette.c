#include "palette.h"

#include "stdio.h"

#include "HW05Lib.h"
#include "savedata.h"
#include "menus.h"

int activePalette;

#define COLOR256(r,g,b) (COLOR((r>>3),(g>>3),(b>>3)))

Palette palettes[] = {
    {"DOWNWELL", {COLOR(30,30,30),COLOR(30,0,0), COLOR(0,15,30), COLOR(1,1,1)}},
    {"MATCHA", {COLOR(30,30,30),COLOR(16,22,0), COLOR(0,15,30), COLOR(1,1,1)}},
    {"AQUA", {COLOR(30,30,30),COLOR(0,15,30), COLOR(30,0,0), COLOR(1,1,1)}},
    {"GBOY", {COLOR256(156,173,135), COLOR256(98,114,78), COLOR256(109,127,86), COLOR256(50,59,40)}}
};

void loadPalette(int index) {
    PALETTE[0] = palettes[index].colors[3];
    PALETTE[1] = palettes[index].colors[3];
    PALETTE[2] = palettes[index].colors[0];
    PALETTE[3] = palettes[index].colors[1];

    PALETTE[1+16] = palettes[index].colors[3];
    PALETTE[2+16] = palettes[index].colors[1];
    PALETTE[3+16] = palettes[index].colors[0];

    SPRITEPALETTE[1] = palettes[index].colors[3];
    SPRITEPALETTE[2] = palettes[index].colors[1];
    SPRITEPALETTE[3] = palettes[index].colors[0];

    activePalette = index;
    setSaveDataEntry(1, index);
}

int getCurrPalette(void) {
    return activePalette;
}

void initPalette(void) {
    activePalette = getSaveDataEntry(1);
    loadPalette(activePalette);

    int numUnlockedPalettes = getSaveDataEntry(3);
    for (int i = 0; i < numUnlockedPalettes; i++) {
        MenuItem *currItem = paletteMenu.items + i;
        currItem->behaviorMode = MENU_FUNCTION_BEHAVIOR;
        currItem->behavior.func = returnFromPaletteMenu;
        currItem->textMode = MENU_CONST_TEXT;
        sprintf(currItem->itemText.text, "%d,%s", i, palettes[i].name);
    }
    paletteMenu.itemCount = numUnlockedPalettes;
}

void unlockPalette(int index) {
    setSaveDataEntry(3, MAX(index, getSaveDataEntry(3)));
    initPalette();
}

void unlockAllPalettes(void) {
    unlockPalette(sizeof(palettes)/sizeof(Palette));
}