#include "palette.hpp"

#include <cstdio>

#include "menus.hpp"
#include "savedata.hpp"

extern "C" {

#include "HW05Lib.h"

}

int activePalette;

#define COLOR256(r,g,b) (COLOR((r>>3),(g>>3),(b>>3)))

const Palette palettes[] = {
    {"DOWNWELL", {COLOR(30,30,30), COLOR(1,1,1),COLOR(30,0,0), COLOR(0,15,30)}},
    {"MATCHA", {COLOR(30,30,30), COLOR(1,1,1),COLOR(16,22,0), COLOR(0,15,30)}},
    {"AQUA", {COLOR(30,30,30), COLOR(1,1,1),COLOR(0,15,30), COLOR(30,0,0)}},
    {"GBOY", {COLOR256(156,173,135), COLOR256(50,59,40), COLOR256(98,114,78), COLOR256(109,127,86)}},
    {"VBOY", {COLOR256(254,17,17), COLOR256(0,0,0), COLOR256(171,0,0), COLOR256(254,107,107)}},
    {"PASTEL", {COLOR256(254,254,254), COLOR256(29,80,194), COLOR256(254,113,96), COLOR256(0,126,254)}},
    {"UMEKOBCHA", {COLOR256(249,238,209), COLOR256(146,164,95), COLOR256(254,53,99), COLOR256(193,239,110)}},
    {"NUCLEAR", {COLOR256(221,254,217), COLOR256(15,58,0), COLOR256(143,204,62), COLOR256(0,0,0)}},
    {"GRANDMA", {COLOR256(254,254,254), COLOR256(99,14,52), COLOR256(83,159,62), COLOR256(254,177,126)}},
    {"GRAYSCALE", {COLOR256(254,254,254), COLOR256(0,0,0), COLOR256(110,110,110), COLOR256(189,189,189)}},
    {"1BIT", {COLOR256(255,255,255), COLOR256(0,0,0), COLOR256(255,255,255), COLOR256(255,255,255)}},
    {"MARS", {COLOR256(228,220,173), COLOR256(97,0,29), COLOR256(234,69,86), COLOR256(0,126,255)}},
    {"BOKJU", {COLOR256(132,132,132), COLOR256(70,70,70), COLOR256(0,0,0), COLOR256(152,152,152)}},
    {"PURPLY", {COLOR256(70,200,254), COLOR256(52,26,18), COLOR256(137,100,180), COLOR256(177,167,154)}},
    {"VIVID", {COLOR256(254,246,254), COLOR256(0,48,70), COLOR256(201,0,0), COLOR256(73,106,180)}},
    {"OLDNCOLD", {COLOR256(254,233,169), COLOR256(4,30,55), COLOR256(105,204,239), COLOR256(0,126,225)}},
    {"MOSSY", {COLOR256(116,245,150), COLOR256(85,47,0), COLOR256(83,133,254), COLOR256(0,126,255)}},
    {"GLOW", {COLOR256(255,0,0), COLOR256(0,0,0), COLOR256(255,255,255), COLOR256(254,166,166)}},
    {"LAVENDER", {COLOR256(201,231,184), COLOR256(96,93,107), COLOR256(145,154,233), COLOR256(80,177,241)}},
    {"FOREST", {COLOR256(126,103,49), COLOR256(0,0,52), COLOR256(45,93,53), COLOR256(0,126,255)}},
    {"WINTER", {COLOR256(210,254,254), COLOR256(60,8,124), COLOR256(101,254,253), COLOR256(0,0,0)}},
    {"ANTIQUE", {COLOR256(166,90,53), COLOR256(49,41,22), COLOR256(239,139,85), COLOR256(210,126,42)}},
    {"DIRTSNOW", {COLOR256(164,164,164), COLOR256(75,75,75), COLOR256(196,59,59), COLOR256(94,127,161)}},
    {"JUNGLE", {COLOR256(254,198,169), COLOR256(0,57,38), COLOR256(155,52,76), COLOR256(86,148,50)}},
    {"SLEEPY", {COLOR256(210,233,218), COLOR256(27,49,77), COLOR256(225,79,127), COLOR256(0,126,255)}},
    {"ZENNYAN", {COLOR256(84,182,55), COLOR256(254,254,254), COLOR256(234,11,197), COLOR256(0,126,255)}},
    {"RHENIUM", {COLOR256(225,150,192), COLOR256(36,44,66), COLOR256(224,246,216), COLOR256(254,76,67)}},
    {"SEWERS", {COLOR256(254,254,254), COLOR256(4,30,55), COLOR256(1,252,254), COLOR256(198,198,198)}},
    {"MAD", {COLOR256(34,31,59), COLOR256(238,154,51), COLOR256(26,224,184), COLOR256(242,38,234)}},
    {"NIKAIDO", {COLOR256(254,254,254), COLOR256(26,26,12), COLOR256(254,147,0), COLOR256(0,126,255)}},
    {"MOLYBDENUM", {COLOR256(183,238,126), COLOR256(30,57,69), COLOR256(11,7,10), COLOR256(108,109,106)}},
    {"SEPIA", {COLOR256(221,170,106), COLOR256(85,30,0), COLOR256(112,167,150), COLOR256(43,75,111)}},
    {"WETLAND", {COLOR256(254,199,169), COLOR256(14,84,63), COLOR256(169,125,57), COLOR256(43,75,111)}},
    {"RGB", {COLOR256(86,228,123), COLOR256(22,43,157), COLOR256(222,0,48), COLOR256(0,0,111)}},
    {"NOSTALGIA", {COLOR256(242,163,163), COLOR256(52,90,121), COLOR256(100,135,201), COLOR256(193,150,254)}},
    {"CHARCOAL", {COLOR256(140,140,140), COLOR256(37,38,39), COLOR256(254,254,254), COLOR256(0,0,0)}},
    {"AERIAL", {COLOR256(118,54,58), COLOR256(220,214,153), COLOR256(254,254,254), COLOR256(220,214,152)}},
    {"HELL", {COLOR256(207,207,207), COLOR256(211,0,0), COLOR256(0,0,0), COLOR256(254,124,1)}}
};


void loadPalette(int index) {
    PALETTE[0] = palettes[index].colors[1];
    PALETTE[1] = palettes[index].colors[1];
    PALETTE[2] = palettes[index].colors[0];
    PALETTE[3] = palettes[index].colors[2];

    PALETTE[1+16] = palettes[index].colors[1];
    PALETTE[2+16] = palettes[index].colors[2];
    PALETTE[3+16] = palettes[index].colors[0];

    SPRITEPALETTE[1] = palettes[index].colors[1];
    SPRITEPALETTE[2] = palettes[index].colors[2];
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
        currItem->func = [](int index){returnFromPaletteMenu();};
        currItem->textMode = MENU_CONST_TEXT;
        sprintf(currItem->itemText.text, "%d,%s", i, palettes[i].name);
    }
    paletteMenu.itemCount = numUnlockedPalettes;
}

void unlockPalette(int index) {
    setSaveDataEntry(3, MAX(index+1, getSaveDataEntry(3)));
    initPalette();
}

void unlockAllPalettes(void) {
    unlockPalette(sizeof(palettes)/sizeof(Palette)-1);
}