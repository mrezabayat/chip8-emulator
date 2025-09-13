#ifndef CHIP8STACK_H
#define CHIP8STACK_H

#include "config.h"
#include <stdint.h>

struct chip8;

typedef struct {
    int16_t stack[CHIP8_STACK_DEPTH];
} chip8_stack;

void chip8_stack_push(struct chip8* chip, int16_t value);
int16_t chip8_stack_pop(struct chip8* chip);

#endif
