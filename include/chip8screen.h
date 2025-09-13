#ifndef CHIP8SCREEN_H
#define CHIP8SCREEN_H

#include "config.h"
#include <stdbool.h>

struct chip8;

typedef struct chip8_screen {
    bool pixels[CHIP8_WIDTH][CHIP8_HEIGHT];
} chip8_screen;

void chip8_screen_pixel_set(struct chip8* chip, int x, int y);
bool chip8_screen_is_pixel_set(const struct chip8* chip, int x, int y);

#endif