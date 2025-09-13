#ifndef CHIP8_H
#define CHIP8_H

#include "chip8keyboard.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8stack.h"
#include "config.h"

typedef struct chip8 {
    chip8_memory    memory;
    chip8_registers registers;
    chip8_stack     stack;
    chip8_keyboard  keyboard;
} chip8;

void chip8_init(chip8 * chip);

#endif