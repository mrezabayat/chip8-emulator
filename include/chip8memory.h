#ifndef CHIP8MEMORY_H
#define CHIP8MEMORY_H

#include "config.h"
#include <stdint.h>

typedef struct {
    int8_t memory[CHIP8_MEMORY_SIZE];
} chip8_memory;

void   chip8_memory_set(chip8_memory* memory, int index, int8_t value);
int8_t chip8_memory_get(const chip8_memory* memory, int index);

#endif