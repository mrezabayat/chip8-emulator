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

void chip8_exec_extended(chip8* chip, uint16_t opcode) {
    uint16_t nnn = opcode & 0x0fff;

    switch (opcode & 0xF000) {
        case 0x1000: /* JP addr: Jump to location nnn */
            chip->registers.PC = nnn;
            break;

        default:
            break;
    }
}

void chip8_exec(chip8* chip, uint16_t opcode) {
    switch (opcode) {
        case 0x00E0: /* CLS: clear the display*/
            chip8_screen_clear(chip);
            break;

        case 0x00EE: /* Ret: Return from a subroutine */
            chip->registers.PC = chip8_stack_pop(chip);
            break;

        default:
            chip8_exec_extended(chip, opcode);
    }
}
