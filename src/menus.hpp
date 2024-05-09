#ifndef MENUS_HPP
#define MENUS_HPP

#include <functional>

enum MENU_TEXT_MODE {MENU_CONST_TEXT, MENU_FUNCTION_TEXT};
enum MENU_BEHAVIOR_MODE {MENU_SUBMENU_BEHAVIOR, MENU_FUNCTION_BEHAVIOR};

typedef struct menu_item {
    enum MENU_TEXT_MODE textMode;
    union {
        char text[20];
        void (*func)(char*, int index);
    } itemText;
    enum MENU_BEHAVIOR_MODE behaviorMode;
    struct menu *submenu;
    std::function<void(int)> func;
    int (*shouldAppear)(void);
} MenuItem;

typedef struct menu {
    MenuItem *items;
    int itemCount;
    void (*draw)(struct menu*, int);
    void (*onSelect)(int);
    void (*onLoad)(void);
} Menu;

extern Menu pauseMenu;
extern Menu optionsMenu;
extern Menu paletteMenu;
extern Menu deleteMenu;
extern Menu cheatsMenu;
extern Menu loseMenu;


extern void returnFromPaletteMenu(void);

#endif