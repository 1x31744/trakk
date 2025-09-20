#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <stdbool.h>

void lcd_init(void);
void lcd_backlight(bool on);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_string(const char *str);


#endif
