#ifndef B_MATRIX_H
#define B_MATRIX_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h> 

typedef struct {
	int row;
	int col;
	bool pressed;
} ButtonPress;

// Function prototypes
void matrix_init();
void matrix_clear_columns();
void matrix_set_column(int col);
bool* matrix_read_rows();
ButtonPress matrix_scan();

#endif
