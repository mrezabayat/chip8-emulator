#include "chip8memory.h"
#include "chip8.h"
#include <assert.h>

void chip8_check_memory_index_bounds(int index) { assert(index >= 0 && index < CHIP8_MEMORY_SIZE); }

void chip8_memory_set(chip8* chip, int index, int8_t value) {
    chip8_check_memory_index_bounds(index);
    chip->memory.data[index] = value;
}

int8_t chip8_memory_get(const chip8* chip, int index) {
    chip8_check_memory_index_bounds(index);
    return chip->memory.data[index];
}
