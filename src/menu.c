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
	
	for (int i = 0;i < currentMenu->itemCount; i++) if (itemArray[i].row > current_r) {
		current_r = itemArray[i].row;
		*selected = i;
		break; // go to the next higher item so break
	}; // current error in this because it goes backwards

}

void nav_down() {
	menuItem *itemArray = currentMenu->items;
	int *selected = &currentMenu->selected_item;
	uint8_t current_r = itemArray[*selected].row;
	
	for (int i = 0;i < currentMenu->itemCount; i++) if (itemArray[i].row < current_r) {
		current_r = itemArray[i].row;
		*selected = i;
		break;
	};

}

void nav_ok() {}

void nav_left() {}

void nav_right() {}


void (*navigation_actions[ROWS][COLUMNS])(void) = {{NULL, nav_up, NULL},
                                                   {nav_left, nav_ok, nav_right},
                                                   {NULL, nav_down, NULL}};

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
                    navigation_actions[r][c]();
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

