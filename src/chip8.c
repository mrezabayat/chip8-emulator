#include "chip8.h"
#include "config.h"
#include <memory.h>

void chip8_init(chip8* chip) {
    memset(chip, 0, sizeof(chip8));
    memcpy(chip, CHIP8_DEFAULT_CHAT_SET, sizeof(CHIP8_DEFAULT_CHAT_SET));
}