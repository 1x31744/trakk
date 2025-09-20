#include "../include/lcd.h"
#include "../include/config.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

int cursorRow;
int cursorColumn;

// --- Pulse Enable for i2c---
void pulse_enable(uint8_t data) {
    uint8_t data_high = data | LCD_E;
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &data_high, 1, false);       // E high
    sleep_us(1);
    uint8_t data_low = data & ~LCD_E;
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &data_low, 1, false);  // E low
    sleep_us(50);
}

// --- Send byte in 4-bit mode ---
void lcd_byte(uint8_t data, bool rs) {
    // first byte to send, upper nibble of data
    uint8_t high = (data & 0xF0);      
    // second byte to send, lower nibble of data
    uint8_t low  = (data << 4) & 0xF0; 

    if (rs) {
        high |= LCD_RS;
        low  |= LCD_RS;
    }

    high |= LCD_BACKLIGHT;
    low  |= LCD_BACKLIGHT;

    pulse_enable(high);
    pulse_enable(low);
}

// --- Backlight control ---
void lcd_backlight(bool on) {
    uint8_t data = on ? LCD_BACKLIGHT : 0x00;
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &data, 1, false);
}

void lcd_init() {
    pulse_enable(0xFF);
    sleep_ms(20);

    // Reset sequence to put LCD into 4-bit mode
    pulse_enable(0x30 | LCD_BACKLIGHT);
    sleep_ms(10);
    pulse_enable(0x30 | LCD_BACKLIGHT);
    sleep_ms(1);
    pulse_enable(0x30 | LCD_BACKLIGHT);
    sleep_us(1);
    pulse_enable(0x20 | LCD_BACKLIGHT); // Set 4-bit mode
    sleep_us(1);

    // Now LCD is in 4-bit mode; send final setup commands

    // Function set: 4-bit, 2 lines, 5x8 font
    lcd_byte(0b00101000, false);

    // Display ON, cursor ON, blink ON
    lcd_byte(0b00001111, false);

    // Entry mode: increment cursor
    //lcd_byte(0b00000110, false);

    // Addres DDRAM with 0 offset 80h
    //lcd_byte(0b10000000, false);
    sleep_ms(2); // Clear display needs extra delay
}

// --- Set cursor for 20x4 LCD ---
void lcd_set_cursor(uint8_t col, uint8_t row) {
	cursorRow = row;
	cursorColumn = col;
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_byte(0x80 | (col + row_offsets[row]), false);
}

void lcd_string(const char *str) {
    while (*str) {
        lcd_byte(*str, LCD_RS); // send each character with RS=1 (data mode)
        str++;
    }
}
