#include "chip8keyboard.h"
#include "chip8.h"
#include <assert.h>

void chip8_check_key_bounds(int key) { assert(key >= 0 && key < CHIP8_TOTAL_KEYS); }

int chip8_keyboard_map(SDL_Keycode key) {
    for (int i = 0; i < CHIP8_TOTAL_KEYS; ++i) {
        if (CHIP8_KEY_MAP[i] == key) {
            return i;
        }
    }
    return -1;
}

void chip8_keyboard_down(chip8* chip, int key) {
    chip->keyboard.keyboard[key] = true;
}

void chip8_keyboard_up(chip8* chip, int key) {
    chip->keyboard.keyboard[key] = false;
}

bool chip8_keyboard_is_down(const chip8* chip, int key) {
    chip8_check_key_bounds(key);
    return chip->keyboard.keyboard[key];
}
