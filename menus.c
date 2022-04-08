#include "menus.h"
#include "stdlib.h"
#include "gamestate.h"
#include "overlay.h"

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
        MENU_FUNCTION_BEHAVIOR,
        NULL
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
        NULL
    }

};

Menu pauseMenu = {
    pauseMenuItems,
    sizeof(pauseMenuItems)/sizeof(MenuItem),
    drawPauseMenu,
    NULL
};