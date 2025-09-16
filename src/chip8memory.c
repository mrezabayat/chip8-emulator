#include "chip8memory.h"
#include "chip8.h"
#include <assert.h>

void chip8_check_memory_index_bounds(int index) { assert(index >= 0 && index < CHIP8_MEMORY_SIZE); }

void chip8_memory_set(chip8* chip, int index, int8_t value) {
    chip8_check_memory_index_bounds(index);
    chip->memory.data[index] = value;
}

uint8_t chip8_memory_get(const chip8* chip, int index) {
    chip8_check_memory_index_bounds(index);
    return chip->memory.data[index];
}

uint16_t chip8_memory_get_two_bytes(const chip8* chip, int index) {
    uint16_t hi = (uint8_t)chip8_memory_get(chip, index);
    uint16_t lo = (uint8_t)chip8_memory_get(chip, index + 1);
    return (uint16_t)((hi << 8) | lo); // big-endian: high byte first
}