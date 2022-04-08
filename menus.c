#include "menus.h"
#include "stdlib.h"
#include "gamestate.h"
#include "overlay.h"
#include "palette.h"

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
    }
};

Menu optionsMenu = {
    optionsMenuItems,
    sizeof(optionsMenuItems)/sizeof(MenuItem),
    drawOptionsMenu,
    NULL,
    NULL
};

void paletteMenuLoad(void) {
    currentMenuIndex = getCurrPalette();
}

void returnFromPaletteMenu(void) {
    loadMenu(&pauseMenu);
    storeSaveData();
}

void drawPaletteMenu(Menu *menu, int selectedIndex) {
    fillOverlayCenter();
    printToOverlay("PALETTE", 12, 1, 0);
    drawScrollingMenu(menu, selectedIndex, 7, 4, 16, 5);
}

MenuItem paletteMenuItems[] = {
    {
        MENU_CONST_TEXT,
        "0.DOWNWELL",
        MENU_FUNCTION_BEHAVIOR,
        returnFromPaletteMenu
    },
    {
        MENU_CONST_TEXT,
        "1.MATCHA",
        MENU_FUNCTION_BEHAVIOR,
        returnFromPaletteMenu
    },
    {
        MENU_CONST_TEXT,
        "2.AQUA",
        MENU_FUNCTION_BEHAVIOR,
        returnFromPaletteMenu
    }
};

Menu paletteMenu = {
    paletteMenuItems,
    sizeof(paletteMenuItems)/sizeof(MenuItem),
    drawPaletteMenu,
    loadPalette,
    paletteMenuLoad
};