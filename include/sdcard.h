#ifndef SDCARD_H
#define SDCARD_H

#include <stdint.h>
#include <stdbool.h>

bool sd_init(uint8_t cs_pin);
int sd_read_block(uint32_t block_addr, uint8_t *buffer);
int sd_write_block(uint32_t block_addr, const uint8_t *buffer);

#endif