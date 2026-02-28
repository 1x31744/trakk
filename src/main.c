#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "../include/b_matrix.h"
#include "../include/config.h"
#include "../include/lcd.h"
#include "../include/menu.h"
#include "../include/sdcard.h"

void init() {
    // Initialize LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Initialize serial & I2C
    stdio_init_all();

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    bi_decl(bi_2pins_with_func(SDA_PIN, SCL_PIN, GPIO_FUNC_I2C));

    sleep_ms(2000);

	gpio_put(LED_PIN, true);

    // SD card init
    sd_init(CS_PIN);

    
}

int main() {
	
    init();

    Menu root;
    
    menuItem noteMenuItems[2] = {
        {.label = "New", .type = MENU_SUBMENU, .col = 0, .row = 3, .submenu = NULL},
        {.label = "Delete", .type = MENU_ACTION, .col = 9, .row = 3, .submenu = NULL}
    };
    
    Menu noteMenu = {
        .title = "Note Menu",
        .items = noteMenuItems,
        .itemCount = 2,
        .parent = NULL,
        .selected_item = 0
    };
    
    menuItem rootItems[3] = {
        {.label = "Add workout", .type = MENU_SUBMENU, .col = 0, .row = 0, .submenu = NULL},
        {.label = "Settings", .type = MENU_SUBMENU, .col = 0, .row = 1, .submenu = NULL},
        {.label = "Notes", .type = MENU_SUBMENU, .col = 0, .row = 2, .submenu = &noteMenu}
    };
    
    root = (Menu){
        .title = "Home",
        .items = rootItems,
        .itemCount = 3,
        .parent = NULL,
        .selected_item = 0
    };
    
    noteMenu.parent = &root;

    menu_init(&root);

    /*
    // Write a test block
    const char *test_data = "this text is written";
    if (sd_write_block(0, test_data) == 0) {
            printf("Block written successfully\n");
    } else {
            printf("Block write failed\n");
    }

    // Read it back
    char read_data[512];
    if (sd_read_block(0, read_data) == 0) {
            lcd_string(read_data);
    } else {
            printf("Block read failed\n");
    }
    */

    absolute_time_t last_led_toggle = get_absolute_time();	
    bool led_state = false;
    matrix_init();
	bool previousRows[ROWS][COLUMNS] = {0};

    while (true) {
        bool rows[ROWS][COLUMNS] = {0};
        matrix_scan(rows);
        menu_handle_input(rows);
    
        bool changed = false;

        // check if anything changed
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLUMNS; c++) {
                if (rows[r][c] != previousRows[r][c]) {
                    changed = true;
                }
            }
        }

        if (changed) {
            printf("\n");

            for (int r = 0; r < ROWS; r++) {
                for (int c = 0; c < COLUMNS; c++) {
                    printf("%d ", rows[r][c]);
                    previousRows[r][c] = rows[r][c];  // update stored state
                }
                printf("\n");
            }

            printf("\n");
        }

        // Toggle LED every 1000ms, non-blocking
        if (absolute_time_diff_us(last_led_toggle, get_absolute_time()) > 1000000){
            led_state = !led_state;
            gpio_put(LED_PIN, led_state);
            last_led_toggle = get_absolute_time();
        }

        sleep_ms(5); // short delay to avoid 100% CPU
    }
}
