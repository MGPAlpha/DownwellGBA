#include "cheats.h"

#include "stdlib.h"
#include "HW05Lib.h"

#include "menus.h"
#include "player.h"
#include "savedata.h"
#include "overlay.h"

int cheatsEnabled;

short konamiCode[] = {
    BUTTON_UP,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_B,
    BUTTON_A,
    BUTTON_START
};
int konamiIndex = 0;

void checkToEnableCheats(void) {
    if (!cheatsEnabled) {
        // mgba_printf("Buttons: %x, OldButtons: %x", !buttons, oldButtons);
        if (~buttons & oldButtons) {
            if (BUTTON_PRESSED(konamiCode[konamiIndex])) {
                konamiIndex++;
                if (konamiIndex >= sizeof(konamiCode)/sizeof(short)) {
                    cheatsEnabled = 1;
                    setSaveDataEntry(5, 1);
                }
            } else {
                konamiIndex = 0;
            }
        }
    }
}

int displayCheatsInMenu(void) {
    return cheatsEnabled;
}

int infiniteAmmoCheat;

void toggleCheat(int index) {
    index--;
    if (cheats[index].enabled) {
        *cheats[index].enabled = !*cheats[index].enabled;
        setSaveDataEntry(index+32, *cheats[index].enabled);
    }
}

void cheatToggleText(char *text, int index) {
    index--;
    if (cheats[index].enabled) {
        sprintf(text, "%s %s", cheats[index].name, *cheats[index].enabled ? "ON" : "OFF");
    }
}

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
        } else if (currCheat->enabled) {
            *currCheat->enabled = getSaveDataEntry(i+32);
            currMenuItem->behavior.func = toggleCheat;
            currMenuItem->textMode = MENU_FUNCTION_TEXT;
            currMenuItem->itemText.func = cheatToggleText;
        }
    }
    cheatsMenu.items[i+1] = (MenuItem){
        MENU_CONST_TEXT,
        "CHEATS OFF",
        MENU_FUNCTION_BEHAVIOR,
        disableCheats
    };
    cheatsMenu.itemCount = i+2;
}

void disableCheats(void) {
    cheatsEnabled = 0;
    setSaveDataEntry(5, 0);
    for (int i = 0; i < NUM_CHEATS; i++) {
        Cheat *currCheat = cheats + i;
        if (currCheat->enabled) {
            *currCheat->enabled = 0;
            setSaveDataEntry(i+32, 0);
        }
    }
    loadMenu(&pauseMenu);
}

void unlockPalettesCheat(void) {
    unlockAllPalettes();
    loadMenu(&paletteMenu);
}

void hpCheat(void) {
    playerHealth++;
    updateHealthDisplay(playerHealth, playerMaxHealth, 0);
}

Cheat cheats[NUM_CHEATS] = {
    {
        "HP +1",
        hpCheat
    },
    {
        "INF AMMO",
        NULL,
        &infiniteAmmoCheat
    },
    {
        "ALL PALETTES",
        unlockPalettesCheat
    }
};