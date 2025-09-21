#include "chip8stack.h"
#include "chip8.h"
#include <assert.h>
#include <stdint.h>

void chip8_stack_push(chip8* chip, uint16_t value) {
    assert(chip->registers.SP < CHIP8_STACK_DEPTH);
    chip->stack.stack[chip->registers.SP++] = value;
}

uint16_t chip8_stack_pop(chip8* chip) {
    assert(chip->registers.SP > 0);
    return chip->stack.stack[--chip->registers.SP];
}
