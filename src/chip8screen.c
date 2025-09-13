#include "chip8screen.h"
#include "chip8.h"
#include "config.h"
#include <assert.h>
#include <stdbool.h>

void chip8_screen_check_bounds(int x, int y) {
    assert(x >= 0 && x < CHIP8_WIDTH && y >= 0 && y < CHIP8_HEIGHT);
}

void chip8_screen_pixel_set(chip8* chip, int x, int y) {
    chip8_screen_check_bounds(x, y);
    chip->screen.pixels[x][y] = true;
}

bool chip8_screen_is_pixel_set(const chip8* chip, int x, int y) {
    chip8_screen_check_bounds(x, y);
    return chip->screen.pixels[x][y];
}
