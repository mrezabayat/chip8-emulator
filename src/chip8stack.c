#include "chip8stack.h"
#include "chip8.h"
#include <assert.h>
#include <stdint.h>

void chip8_check_stack_bounds(const chip8* chip) {
    assert(chip->registers.SP >= 0 && chip->registers.SP < CHIP8_STACK_DEPTH);
}

void chip8_stack_push(chip8* chip, int16_t value) {
    ++chip->registers.SP;
    chip8_check_stack_bounds(chip);
    chip->stack.stack[chip->registers.SP] = value;
}

int16_t chip8_stack_pop(chip8* chip) {
    chip8_check_stack_bounds(chip);
    int16_t top = chip->stack.stack[chip->registers.SP];
    --chip->registers.SP;
    return top;
}
