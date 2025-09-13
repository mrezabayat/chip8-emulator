#include "chip8.h"
#include <memory.h>

void chip8_init(chip8* chip) { memset(chip, 0, sizeof(chip8)); }