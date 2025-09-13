#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include "config.h"
#include <stdbool.h>

struct chip8;

typedef struct chip8_keyboard {
    bool keys[CHIP8_TOTAL_KEYS];

} chip8_keyboard;

int  chip8_keyboard_map(SDL_Keycode key);
void chip8_keyboard_down(struct chip8* chip, int key);
void chip8_keyboard_up(struct chip8* chip, int key);
bool chip8_keyboard_is_down(const struct chip8* chip, int key);

#endif