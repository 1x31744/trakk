#ifndef MENU_H
#define MENU_H

#include <stdio.h>

typedef enum {
    MENU_ACTION, 
    MENU_SUBMENU,
    MENU_INPUT
} MenuItemType;

typedef void (*MenuAction)(const char *name);

typedef struct Menu Menu;

typedef struct {
    const char *label;
    MenuItemType type;
    union {
        Menu *submenu; 
        MenuAction action;
    };
} menuItem;

struct Menu {
    const char *title;
    menuItem *items;
    size_t itemCount;
    Menu *parent; 
};

// API
void menu_init(Menu *root);
void menu_draw(void);
void menu_handle_input(int action);


// Action enums (from buttons)
enum {
    ACTION_NONE,
    ACTION_UP,
    ACTION_DOWN,
    ACTION_ENTER,
    ACTION_BACK
};

#endif