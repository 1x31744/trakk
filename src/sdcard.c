#include "../include/sdcard.h"
#include "../include/config.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

// static to keep private to this file
static uint cs_pin;
static inline uint8_t spi_txrx(uint8_t data) {
    uint8_t rx;
    spi_write_read_blocking(spi0, &data, &rx, 1);
    return rx;
}

static uint8_t sd_wait_response(void) {
    uint8_t r;
    int timeout = 0xFFFF;
    do {
        r = spi_txrx(0xFF);
    } while ((r == 0xFF) && --timeout);
    return r;
}

static uint8_t sd_send_command(uint8_t cmd, uint32_t arg, uint8_t crc) {
    spi_txrx(0xFF);
    gpio_put(cs_pin, 0);

    spi_txrx(0x40 | cmd);
    spi_txrx((arg >> 24) & 0xFF);
    spi_txrx((arg >> 16) & 0xFF);
    spi_txrx((arg >> 8) & 0xFF);
    spi_txrx(arg & 0xFF);
    spi_txrx(crc);

    return sd_wait_response();
}

bool sd_init(uint8_t cs) {
    cs_pin = cs;

    spi_init(spi0, 200 * 1000);
    gpio_set_function(20, GPIO_FUNC_SPI);
    gpio_set_function(23, GPIO_FUNC_SPI);
    gpio_set_function(22, GPIO_FUNC_SPI);

    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);

    sleep_ms(100);

    // Send 80 dummy clocks
    for (int i = 0; i < 10; i++) spi_txrx(0xFF);

    // CMD0 loop
    uint8_t r;
    do {
        r = sd_send_command(0, 0x00000000, 0x95);
        gpio_put(cs_pin, 1);
    } while (r != 0x01);

    // CMD8 check
    r = sd_send_command(8, 0x000001AA, 0x87);
    gpio_put(cs_pin, 1);
    if (r != 0x01 && r != 0x05) {
        return false; // unsupported
    }

    // ACMD41 init loop
    do {
        sd_send_command(55, 0, 0x65);
        r = sd_send_command(41, 0x40000000, 0x77);
        gpio_put(cs_pin, 1);
        sleep_ms(50);
    } while (r != 0x00);

    return true;
}

int sd_read_block(uint32_t block_addr, uint8_t *buffer) {
    uint8_t r = sd_send_command(17, block_addr, 0xFF);
    if (r != 0x00) {
        gpio_put(cs_pin, 1);
        return -1;
    }

    uint8_t token;
    int timeout = 0xFFFF;
    do {
        token = spi_txrx(0xFF);
    } while (token == 0xFF && --timeout);

    if (token != 0xFE) {
        gpio_put(cs_pin, 1);
        return -2;
    }

    for (int i = 0; i < 512; i++) {
        buffer[i] = spi_txrx(0xFF);
    }

    spi_txrx(0xFF); // discard CRC
    spi_txrx(0xFF);

    gpio_put(cs_pin, 1);
    return 0;
}

int sd_write_block(uint32_t block_addr, const uint8_t *buffer) {
    uint8_t r = sd_send_command(24, block_addr, 0xFF);
    if (r != 0x00) {
        gpio_put(cs_pin, 1);
        return -1;
    }

    spi_txrx(0xFE);
    for (int i = 0; i < 512; i++) {
        spi_txrx(buffer[i]);
    }
    spi_txrx(0xFF);
    spi_txrx(0xFF);

    r = sd_wait_response();
    if ((r & 0x1F) != 0x05) {
        gpio_put(cs_pin, 1);
        return -2;
    }

    while (spi_txrx(0xFF) == 0x00);

    gpio_put(cs_pin, 1);
    return 0;
}
