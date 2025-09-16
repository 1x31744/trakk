#include "../include/config.h"
#include "../include/lcd.h"
#include "../include/menu.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

static Menu *currentMenu = NULL;
static int currentIndex = 0;
bool previous[ROWS][COLS];
Mode currentMode = NAVIGATION;

void (*navigation_actions[ROWS][COLUMNS])(void) = {{nullptr, nav_up, nullptr},
                                                   {nav_left, nav_ok, nav_right},
                                                   {nullptr, nav_down, nullptr}}

// Initialize menu system with root menu
void menu_init(Menu * root) {
    currentMenu = root;
    currentIndex = 0;

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

        if (currentMenu->items[i].selected) {
            lcd_string('>')
        }

        lcd_string(currentMenu->items[i].label);
    }
}

void menu_handle_input(int[ROWS][COLUMNS currentMatrix) {

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++) {
            if (current_matrix[r][c] != previous[r][c]) {
                switch (current_mode) {
                case NAVIGATION:
                    navigation_actions[r][c]();
                    break;
                case TEXT_ENTRY;
					break;
				}
            }
            previous[r][c] = current_matrix[r][c];
        }
    }
}

void nav_up() {
	menuItem *itemArray = currentMenu->items;
	int current_r = itemArray[selected].row;
	int current_c = itemArray[selected].col;
	
	for (int i = 0;i < currentMenu->itemCount; i++) if (itemArray[i].row > current_r) current_r = itemArray[i];

}

void nav_down() {
	menuItem *itemArray = currentMenu->items;
	int current_r = itemArray[selected].row;
	int current_c = itemArray[selected].col;
	
	for (int i = 0;i < currentMenu->itemCount; i++) if (itemArray[i].row < current_r) current_r = itemArray[i];

}
