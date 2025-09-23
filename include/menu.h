#ifndef MENU_H
#define MENU_H

#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum { MENU_ACTION, MENU_SUBMENU, MENU_INPUT } MenuItemType;

typedef void (*MenuAction)(const char *name);

typedef struct Menu Menu;

typedef struct {
    const char *label;
    MenuItemType type;
    uint8_t col;
    uint8_t row;
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
    int selected_item;
};

// API
void menu_init(Menu *root);
void menu_draw(void);
void menu_handle_input(int r, int c);

// Action enums (from buttons)
enum { ACTION_NONE, ACTION_UP, ACTION_DOWN, ACTION_ENTER, ACTION_BACK };

typedef enum { NAVIGATION, TEXT_ENTRY } Mode;

typedef struct {
	const char *text;
	int timesPressed;
	absolute_time_t lastPressed;
} TextButton;

void text_actions(int row, int column);

#endif
