#ifndef CHIP8STACK_H
#define CHIP8STACK_H

#include "config.h"
#include <stdint.h>

struct chip8;

typedef struct {
    uint16_t stack[CHIP8_STACK_DEPTH];
} chip8_stack;

void     chip8_stack_push(struct chip8* chip, uint16_t value);
uint16_t chip8_stack_pop(struct chip8* chip);

#endif
