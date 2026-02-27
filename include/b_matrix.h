#ifndef B_MATRIX_H
#define B_MATRIX_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> 
#include "../include/config.h"

// Function prototypes
void matrix_init();
void matrix_clear_columns();
void matrix_set_column(int col);
bool* matrix_read_rows();
void matrix_scan(bool rows[ROWS][COLUMNS]);

#endif
