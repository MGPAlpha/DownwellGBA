#include "menus.h"
#include "stdlib.h"
#include "gamestate.h"
#include "overlay.h"
#include "palette.h"
#include "savedata.h"

void drawPauseMenu(Menu *menu, int selectedIndex) {
    fillOverlayCenter();
    printToOverlay("PAUSED", 12, 1, 0);
    drawMenu(menu, selectedIndex, 7, 4, 16, 8);
}

MenuItem pauseMenuItems[] = {
    {
        MENU_CONST_TEXT,
        "RESUME",
        MENU_FUNCTION_BEHAVIOR,
        unpause
    },
    {
        MENU_CONST_TEXT,
        "QUICK START",
        MENU_FUNCTION_BEHAVIOR,
        NULL
    },
    {
        MENU_CONST_TEXT,
        "STYLE SELECT",
        MENU_FUNCTION_BEHAVIOR,
        NULL
    },
    {
        MENU_CONST_TEXT,
        "PALETTE",
        MENU_SUBMENU_BEHAVIOR,
        &paletteMenu
    },
    {
        MENU_CONST_TEXT,
        "STATS",
        MENU_SUBMENU_BEHAVIOR,
        NULL
    },
    {
        MENU_CONST_TEXT,
        "OPTION",
        MENU_SUBMENU_BEHAVIOR,
        &optionsMenu
    }

};

Menu pauseMenu = {
    pauseMenuItems,
    sizeof(pauseMenuItems)/sizeof(MenuItem),
    drawPauseMenu,
    NULL,
    NULL
};

void drawOptionsMenu(Menu *menu, int selectedIndex) {
    fillOverlayCenter();
    printToOverlay("OPTIONS", 12, 1, 0);
    drawMenu(menu, selectedIndex, 7, 4, 16, 8);
}

MenuItem optionsMenuItems[] = {
    {
        MENU_CONST_TEXT,
        "BACK",
        MENU_SUBMENU_BEHAVIOR,
        &pauseMenu
    },
    {
        MENU_CONST_TEXT,
        "DELETE SAVE",
        MENU_SUBMENU_BEHAVIOR,
        &deleteMenu
    }
};

Menu optionsMenu = {
    optionsMenuItems,
    sizeof(optionsMenuItems)/sizeof(MenuItem),
    drawOptionsMenu,
    NULL,
    NULL
};

void deleteSaveAndRestart(void) {
    resetSaveData();
    initSurface();
}

void drawDeleteMenu(Menu *menu, int selectedIndex) {
    fillOverlayCenter();
    printToOverlay("DELETE ALL", 10, 1, 0);
    printToOverlay("SAVE DATA?", 10, 2, 0);
    drawMenu(menu, selectedIndex, 6, 9, 16, 2);
}

MenuItem deleteMenuItems[] = {
    {
        MENU_CONST_TEXT,
        "GO BACK",
        MENU_SUBMENU_BEHAVIOR,
        &optionsMenu
    },
    {
        MENU_CONST_TEXT,
        "DELETE SAVE DATA",
        MENU_FUNCTION_BEHAVIOR,
        deleteSaveAndRestart
    }
};

Menu deleteMenu = {
    deleteMenuItems,
    sizeof(deleteMenuItems)/sizeof(MenuItem),
    drawDeleteMenu,
    NULL,
    NULL
};

void paletteMenuLoad(void) {
    currentMenuIndex = getCurrPalette();
}

void returnFromPaletteMenu(void) {
    loadMenu(&pauseMenu);
}

void drawPaletteMenu(Menu *menu, int selectedIndex) {
    fillOverlayCenter();
    printToOverlay("PALETTE", 12, 1, 0);
    drawScrollingMenu(menu, selectedIndex, 7, 4, 16, 5);
}

MenuItem paletteMenuItems[38] = {
    
};

Menu paletteMenu = {
    paletteMenuItems,
    0,
    drawPaletteMenu,
    loadPalette,
    paletteMenuLoad
};