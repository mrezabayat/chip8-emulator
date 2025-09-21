#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include "config.h"
#include <stdint.h>

struct chip8;

typedef struct {
    uint8_t data[CHIP8_MEMORY_SIZE];
} chip8_memory;

void     chip8_memory_set(struct chip8* chip, int index, uint8_t value);
uint8_t  chip8_memory_get(const struct chip8* chip, int index);
uint16_t chip8_memory_get_two_bytes(const struct chip8* chip, int index);

#endif
