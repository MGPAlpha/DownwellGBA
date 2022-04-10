#include "cheats.h"

#include "stdlib.h"

#include "menus.h"
#include "savedata.h"

int cheatsEnabled;

void initCheats(void) {
    cheatsEnabled = getSaveDataEntry(5);
    cheatsMenu.items[0] = (MenuItem){
        MENU_CONST_TEXT,
        "BACK",
        MENU_SUBMENU_BEHAVIOR,
        &pauseMenu
    };
    int i = 0;
    for (; i < NUM_CHEATS; i++) {
        Cheat *currCheat = cheats + i;
        MenuItem *currMenuItem = cheatsMenu.items + 1 + i;
        if (!currCheat->name[0]) break;
        currMenuItem->behaviorMode = MENU_FUNCTION_BEHAVIOR;
        if (currCheat->invoke) {
            currMenuItem->textMode = MENU_CONST_TEXT;
            sprintf(currMenuItem->itemText.text, "%s", currCheat->name);
            currMenuItem->behavior.func = currCheat->invoke;
        }
    }
    cheatsMenu.items[i+1] = (MenuItem){
        MENU_CONST_TEXT,
        "DISABLE CHEATS",
        MENU_FUNCTION_BEHAVIOR,
        NULL
    };
    cheatsMenu.itemCount = i+2;
}

void unlockPalettesCheat(void) {
    unlockAllPalettes();
    loadMenu(&paletteMenu);
}

Cheat cheats[NUM_CHEATS] = {
    {
        "UNLOCK PALETTES",
        unlockPalettesCheat
    }
};