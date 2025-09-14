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
}