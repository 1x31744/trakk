#include "../include/config.h"
#include "../include/lcd.h"
#include "../include/menu.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

static Menu *currentMenu = NULL;
bool previous[ROWS][COLUMNS];
Mode currentMode = NAVIGATION;

void nav_up() {
	menuItem *itemArray = currentMenu->items;
	int *selected = &currentMenu->selected_item;
	uint8_t current_r = itemArray[*selected].row;
	uint8_t next_r = 0;

    for (int i = 0; i < currentMenu->itemCount; i++) {
        if (itemArray[i].row < current_r && itemArray[i].row >= next_r) {
            next_r = itemArray[i].row;
            *selected = i;
        }
    }

}

void nav_down() {
	menuItem *itemArray = currentMenu->items;
	int *selected = &currentMenu->selected_item;
	uint8_t current_r = itemArray[*selected].row;
	
    for (int i = 0; i < currentMenu->itemCount; i++) {
        if (itemArray[i].row > current_r) {
            *selected = i;
            break;
        }
    }

}

void nav_ok() {
    menuItem *item = &currentMenu->items[currentMenu->selected_item];

    switch (item->type) {
    case MENU_SUBMENU:
            currentMenu->parent = currentMenu;
            currentMenu = item->submenu;
            break;

        case MENU_ACTION:
            //item->action();
            break;
    }
}

void nav_left() {
	menuItem *itemArray = currentMenu->items;
	int *selected = &currentMenu->selected_item;
	uint8_t current_c = itemArray[*selected].col;
	uint8_t next_c = 0;

    for (int i = 0; i < currentMenu->itemCount; i++) {
        if (itemArray[i].col < current_c && itemArray[i].col >= next_c) {
            next_c = itemArray[i].col;
            *selected = i;
        }
    }
}

void nav_right() {
    menuItem *itemArray = currentMenu->items;
	int *selected = &currentMenu->selected_item;
	uint8_t current_c = itemArray[*selected].col;
	
    for (int i = 0; i < currentMenu->itemCount; i++) {
        if (itemArray[i].col > current_c) {
            *selected = i;
            break;
        }
    }
}

void nav_back() {
    if (currentMenu->parent != NULL) {
        currentMenu = currentMenu->parent;
    }

}


void (*navigation_actions[ROWS][COLUMNS])(void) = {
    {NULL, NULL, nav_up,   NULL},
    {NULL, nav_left, nav_ok, nav_right},
    {NULL, nav_back, nav_down, NULL},
    {NULL, NULL, NULL,     NULL}
};

// Initialize menu system with root menu
void menu_init(Menu * root) {
    currentMenu = root; 

    lcd_init();
    lcd_backlight(true);

    menu_draw();
}

void menu_draw(void) {
    if (!currentMenu)
        return;

    for (uint8_t r = 0; r < 4; r++) {
        lcd_set_cursor(0, r);
        lcd_string("                    ");
    }

    for (size_t i = 0; i < currentMenu->itemCount; i++) {
        lcd_set_cursor(currentMenu->items[i].col, currentMenu->items[i].row);

        if (currentMenu->selected_item == i) {
            lcd_string(">");
        }

        lcd_string(currentMenu->items[i].label);
    }
}

void menu_handle_input(bool currentMatrix[ROWS][COLUMNS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            if (currentMatrix[r][c] && !previous[r][c]) {
                switch (currentMode) {
                case NAVIGATION:
                    if (navigation_actions[r][c] != NULL) {
                        navigation_actions[r][c]();
                    }
					menu_draw();
                    break;
				case TEXT_ENTRY:
					break;
				}
            }
            previous[r][c] = currentMatrix[r][c];
        }
    }
}

