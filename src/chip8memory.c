#include "chip8memory.h"
#include <assert.h>

void chip8_check_memory_index_bounds(int index) { assert(index >= 0 && index < CHIP8_MEMORY_SIZE); }

void chip8_memory_set(chip8_memory* memory, int index, int8_t value) {
    chip8_check_memory_index_bounds(index);
    memory->memory[index] = value;
}

int8_t chip8_memory_get(const chip8_memory* memory, int index) {
    chip8_check_memory_index_bounds(index);
    return memory->memory[index];
}
