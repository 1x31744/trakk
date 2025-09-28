#include "../include/config.h"
#include "../include/lcd.h"
#include "../include/menu.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"


static Menu *currentMenu = NULL;
static int currentIndex = 0;
Mode currentMode = NAVIGATION;
extern int cursorRow;
extern int cursorColumn;

void nav_up() {
	menuItem *itemArray = currentMenu->items;
	int *selected = &currentMenu->selected_item;
	uint8_t current_r = itemArray[*selected].row;
	
	for (int i = 0;i < currentMenu->itemCount; i++) if (itemArray[i].row > current_r) {
		current_r = itemArray[i].row;
		*selected = i;
	};

}

void nav_down() {
	menuItem *itemArray = currentMenu->items;
	int *selected = &currentMenu->selected_item;
	uint8_t current_r = itemArray[*selected].row;
	
	for (int i = 0;i < currentMenu->itemCount; i++) if (itemArray[i].row < current_r) {
		current_r = itemArray[i].row;
		*selected = i;
	};

}

void nav_ok() {}

void nav_left() {}

void nav_right() {}


void (*navigation_actions[ROWS][COLUMNS])(void) = {{NULL, nav_up, NULL},
                                                   {nav_left, nav_ok, nav_right},
                                                   {NULL, nav_down, NULL}};

TextButton textButtons[ROWS][COLUMNS] = 
{{{"1abc", 0, 0}, {"2def", 0, 0}, {"3ghi", 0, 0}},
{{"4jkl", 0, 0,}, {"5mno", 0, 0},{"6pqr", 0, 0}},
{{"7stu", 0, 0}, {"8vwx", 0, 0}, {"9yz0", 0, 0}}};

void text_actions(int row, int column) { //error, prints first item of each text because updates each button on a press, maybe change the input function to just output r and c
	int *timesPressed = &textButtons[row][column].timesPressed;
	absolute_time_t *lastPressed = &textButtons[row][column].lastPressed;

	if (absolute_time_diff_us(*lastPressed, get_absolute_time()) < 500000) {
		//get cursor x-1, and draw character there with the index of timesPressed (increase and wrap times pressed)
		lcd_set_cursor(cursorColumn - 1, cursorRow);
	} else {
		//draw character normally
		*timesPressed = 0;
	}

	lcd_string((char[]){textButtons[row][column].text[*timesPressed], '\0'});
	*timesPressed = (*timesPressed + 1) % 3;
	*lastPressed = get_absolute_time();

}

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
	//4 is used for the number of lines of the lcd
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

void menu_handle_input(int r, int c) {

	switch (currentMode) {
		case NAVIGATION:
			navigation_actions[r][c]();
			menu_draw();
			break;
		case TEXT_ENTRY:
			text_actions(r, c);
			//eventually will need to turn text into an intem and draw in memory draw
			break;
	}
}
