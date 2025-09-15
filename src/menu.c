#include "../include/menu.h"
#include "../include/lcd.h"
#include "../include/config.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"




static Menu *currentMenu = NULL;
static int currentIndex = 0;

// Initialize menu system with root menu
void menu_init(Menu *root) {
    currentMenu = root;
    currentIndex = 0;

    lcd_init();
    lcd_backlight(true);

    menu_draw();
}

void menu_draw(void) {
	if (!currentMenu) return;

	for (uint8_t r = 0; r < 4; r++) {
		lcd_set_cursor(0,r);
		lcd_string("                    ");
	}

	for (size_t i = 0; i < currentMenu->itemCount; i++) {
		lcd_set_cursor(currentMenu->items[i].col, currentMenu->items[i].row);
		
		if (currentMenu->items[i].selected) {
			// do some lcd selection stuff
		}

		lcd_string(currentMenu->items[i].label);

	}
}

void handle_input(int action) {
	//recieve input from main
}
