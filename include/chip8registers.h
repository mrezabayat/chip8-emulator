#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include "config.h"
#include <stdint.h>

typedef struct {
    int8_t  registers[CHIP8_TOTAL_REGISTERS];
    int16_t I;
    int8_t  delay_timer;
    int8_t  sound_timer;
    int16_t PC;
    int8_t  SP;
} chip8_registers;

#endif
