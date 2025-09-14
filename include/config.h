#ifndef CONFIG_H
#define CONFIG_H

#include "hardware/i2c.h"   // for i2c0, i2c1
#include "hardware/spi.h"   // if you use spi0/spi1 in config
#include "pico/stdlib.h"    // optional, for GPIO numbers

// I2C setup
#define I2C_PORT i2c0
#define SDA_PIN 0
#define SCL_PIN 1
#define LCD_ADDR 0x27

// LCD backpack mapping
#define LCD_RS        0x01
#define LCD_E         0x04
#define LCD_BACKLIGHT 0x08

// SD card
#define CS_PIN 10

#define COLUMN_1 2
#define COLUMN_2 3
#define COLUMN_3 4

#define ROW_1 5
#define ROW_2 6
#define ROW_3 7

// LED pin
#define LED_PIN 17

#endif
