#ifndef MENUS_H
#define MENUS_H

enum MENU_TEXT_MODE {MENU_CONST_TEXT, MENU_FUNCTION_TEXT};
enum MENU_BEHAVIOR_MODE {MENU_SUBMENU_BEHAVIOR, MENU_FUNCTION_BEHAVIOR};

typedef struct menu_item {
    enum MENU_TEXT_MODE textMode;
    union {
        char *text;
        void (*func)(char*);
    } itemText;
    enum MENU_BEHAVIOR_MODE behaviorMode;
    union {
        struct menu *submenu;
        void (*func)(void);
    } behavior;
} MenuItem;

typedef struct menu {
    MenuItem *items;
    int itemCount;
    void (*draw)(struct menu*, int);
    void (*onSelect)(int);
} Menu;

extern Menu pauseMenu;

#endif