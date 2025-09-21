#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H

#include "config.h"
#include <stdint.h>

typedef struct {
    uint8_t  registers[CHIP8_TOTAL_REGISTERS];
    uint16_t I;
    uint8_t  delay_timer;
    uint8_t  sound_timer;
    uint16_t PC;
    uint8_t  SP;
} chip8_registers;

#endif
