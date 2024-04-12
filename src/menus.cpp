#include "menus.hpp"

#include "gamestate.hpp"
#include <cstdlib>

#include "palette.hpp"
#include "cheats.hpp"
#include "overlay.hpp"
#include "gem.hpp"
#include "savedata.hpp"

extern "C" {
    
    #include "unlocks.h"

}


void drawPauseMenu(Menu *menu, int selectedIndex) {
    fillOverlayCenter();
    printToOverlay("PAUSED", 12, 1, 0);
    drawMenu(menu, selectedIndex, 7, 4, 16, 8);

    int nextUnlockIndex = getNextUnlockIndex(lifetimeGems);
    int gemMax = unlocks[nextUnlockIndex].quota;
    int prevGemMax = nextUnlockIndex > 0 ? unlocks[nextUnlockIndex-1].quota : 0;
    drawGemProgress(lifetimeGems, prevGemMax, gemMax, 13);
}

int showOnlyOnSurface(void) {
    return unpauseState == GAME_SURFACE;
}

int showOnlyInGame(void) {
    return unpauseState == GAME_PLAY;
}

void quickStart(void) {

}

MenuItem pauseMenuItems[] = {
    {
        MENU_CONST_TEXT,
        "RESUME",
        MENU_FUNCTION_BEHAVIOR,
        NULL,
        [](int index){unpause();}
    },
    {
        MENU_CONST_TEXT,
        "QUICK START",
        MENU_FUNCTION_BEHAVIOR,
        NULL,
        [](int index){initGame();},
        showOnlyOnSurface
    },
    {
        MENU_CONST_TEXT,
        "RETRY",
        MENU_FUNCTION_BEHAVIOR,
        NULL,
        [](int index){initGame();},
        showOnlyInGame
    },
    {
        MENU_CONST_TEXT,
        "INSTRUCTIONS",
        MENU_FUNCTION_BEHAVIOR,
        NULL,
        [](int index){initInstructions();},
    },
    // {
    //     MENU_CONST_TEXT,
    //     "STYLE SELECT",
    //     MENU_FUNCTION_BEHAVIOR,
    //     NULL
    // },
    {
        MENU_CONST_TEXT,
        "PALETTE",
        MENU_SUBMENU_BEHAVIOR,
        &paletteMenu,
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
        &optionsMenu,
        NULL
    },
    {
        MENU_CONST_TEXT,
        "CHEATS",
        MENU_SUBMENU_BEHAVIOR,
        &cheatsMenu,
        NULL,
        displayCheatsInMenu
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
        &pauseMenu,
        NULL
    },
    {
        MENU_CONST_TEXT,
        "DELETE SAVE",
        MENU_SUBMENU_BEHAVIOR,
        &deleteMenu,
        NULL
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
        &optionsMenu,
        NULL
    },
    {
        MENU_CONST_TEXT,
        "DELETE SAVE DATA",
        MENU_FUNCTION_BEHAVIOR,
        NULL,
        [](int index){deleteSaveAndRestart();},
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

MenuItem paletteMenuItems[50] = {
    
};

Menu paletteMenu = {
    paletteMenuItems,
    0,
    drawPaletteMenu,
    loadPalette,
    paletteMenuLoad
};

void drawCheatsMenu(Menu *menu, int selectedIndex) {
    fillOverlayCenter();
    printToOverlay("CHEATS", 12, 1, 0);
    drawMenu(menu, selectedIndex, 7, 4, 16, 5);
}

MenuItem cheatsMenuItems[NUM_CHEATS];

Menu cheatsMenu = {
    cheatsMenuItems,
    sizeof(cheatsMenuItems)/sizeof(MenuItem),
    drawCheatsMenu,
    NULL,
    NULL
};

void drawLoseMenu(Menu *menu, int selectedIndex) {
    fillOverlayRect(5, 15, 20, 2, 32*9+1);
    drawMenu(menu, selectedIndex, 11, 15, 16, 5);
}

MenuItem loseMenuItems[] = {
    {
        MENU_CONST_TEXT,
        "RETRY",
        MENU_FUNCTION_BEHAVIOR,
        NULL,
        [](int index){initGame();},
    },
    {
        MENU_CONST_TEXT,
        "SURFACE",
        MENU_FUNCTION_BEHAVIOR,
        NULL,
        [](int index){initSurface();},
    }
};

Menu loseMenu = {
    loseMenuItems,
    sizeof(loseMenuItems)/sizeof(MenuItem),
    drawLoseMenu,
    NULL,
    NULL
};