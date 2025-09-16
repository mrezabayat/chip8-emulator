#include "chip8screen.h"
#include "chip8.h"
#include "config.h"
#include <assert.h>
#include <memory.h>
#include <stdbool.h>

void chip8_screen_pixel_set(chip8* chip, int x, int y) {
    chip->screen.pixels[x % CHIP8_WIDTH][y % CHIP8_HEIGHT] ^= true;
}

bool chip8_screen_is_pixel_set(const chip8* chip, int x, int y) {
    return chip->screen.pixels[x % CHIP8_WIDTH][y % CHIP8_HEIGHT];
}

void chip8_screen_draw_sprite(chip8* chip, int x, int y, const uint8_t* sprite, int num) {

    for (int ly = 0; ly < num; ++ly) {
        char line = sprite[ly];
        for (int lx = 0; lx < 8; ++lx) {
            if ((line & (0b10000000 >> lx)) == 0)
                continue;
            if (chip8_screen_is_pixel_set(chip, lx + x, ly + y)) {
                chip->registers.registers[0xf] = 1;
            }
            chip8_screen_pixel_set(chip, lx + x, ly + y);
        }
    }
}

void chip8_screen_clear(chip8* chip) {
    memset(&chip->screen.pixels, 0, sizeof(chip->screen.pixels));
}
