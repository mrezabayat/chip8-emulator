#include "chip8.h"
#include "config.h"
#include <assert.h>
#include <memory.h>
#include <stddef.h>

void chip8_init(chip8* chip) {
    memset(chip, 0, sizeof(chip8));
    memcpy(chip->memory.data, CHIP8_DEFAULT_CHAT_SET, sizeof(CHIP8_DEFAULT_CHAT_SET));
}

void chip8_load(chip8* chip, const char* buff, size_t size) {
    assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip->memory.data[CHIP8_PROGRAM_LOAD_ADDRESS], buff, size);
    chip->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

void chip8_exec(chip8* chip, int16_t opcode) { chip->registers.PC += 2; }
