#ifndef CHIP8_H
#define CHIP8_H

#include "chip8keyboard.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8screen.h"
#include "chip8stack.h"
#include "config.h"

typedef struct chip8 {
    chip8_memory    memory;
    chip8_registers registers;
    chip8_stack     stack;
    chip8_keyboard  keyboard;
    chip8_screen    screen;
} chip8;

void chip8_init(chip8* chip);
void chip8_load(chip8* chip, const char* buff, size_t size);
void chip8_exec(chip8* chip, uint16_t opcode);

#endif