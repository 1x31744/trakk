#ifndef B_MATRIX_H
#define B_MATRIX_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> 

// Function prototypes
void matrix_init();
void matrix_clear_columns();
void matrix_set_column(int col);
bool* matrix_read_rows();
void matrix_scan(bool rows[3][3]);

#endif
