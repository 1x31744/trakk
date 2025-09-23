#include "../include/b_matrix.h"
#include "../include/config.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

void matrix_init() {
    // Initialize row pins as input
    gpio_init(ROW_1);
    gpio_set_dir(ROW_1, GPIO_IN);
    gpio_pull_up(ROW_1);
    

    gpio_init(ROW_2);
    gpio_set_dir(ROW_2, GPIO_IN);
    gpio_pull_up(ROW_2);

    gpio_init(ROW_3);
    gpio_set_dir(ROW_3, GPIO_IN);
    gpio_pull_up(ROW_3);

    // Initialize column pins as outputs
    gpio_init(COLUMN_1);
    gpio_set_dir(COLUMN_1, GPIO_OUT);
    gpio_put(COLUMN_1, 1);

    gpio_init(COLUMN_2);
    gpio_set_dir(COLUMN_2, GPIO_OUT);
    gpio_put(COLUMN_2, 1);

    gpio_init(COLUMN_3);
    gpio_set_dir(COLUMN_3, GPIO_OUT);
    gpio_put(COLUMN_3, 1);
}

ButtonPress matrix_scan(void) {
    for (int col = 0; col < 3; col++) {
        matrix_clear_columns();
        matrix_set_column(col);
        //sleep_us(100); // Allow time for the column to stabilize
        bool* row_data = matrix_read_rows();
        for (int row = 0; row < 3; row++) {
            if (row_data[row]) {
				return (ButtonPress){row, col, true};
            } else {
				return (ButtonPress){row, col, true};
			}
        }
    }
}

bool* matrix_read_rows() {
    static bool rows[3];
    printf("Reading rows: %d %d %d\n", gpio_get(ROW_1), gpio_get(ROW_2), gpio_get(ROW_3));
    rows[0] = (gpio_get(ROW_1) == 0);
    rows[1] = (gpio_get(ROW_2) == 0);
    rows[2] = (gpio_get(ROW_3) == 0);
    return rows;
}

void matrix_clear_columns() {
    // idle HIGH
    gpio_put(COLUMN_1, 1);
    gpio_put(COLUMN_2, 1);
    gpio_put(COLUMN_3, 1);
}

void matrix_set_column(int col) {
    // active LOW
    switch (col) {
        case 0: gpio_put(COLUMN_1, 0); break;
        case 1: gpio_put(COLUMN_2, 0); break;
        case 2: gpio_put(COLUMN_3, 0); break;
    }
}
