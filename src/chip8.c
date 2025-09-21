#include "chip8.h"
#include "config.h"
#include <assert.h>
#include <memory.h>
#include <stddef.h>
#include <time.h>

#include "SDL2/SDL.h"

void chip8_init(chip8* chip) {
    memset(chip, 0, sizeof(chip8));
    memcpy(chip->memory.data, CHIP8_DEFAULT_CHAT_SET, sizeof(CHIP8_DEFAULT_CHAT_SET));
}

void chip8_load(chip8* chip, const char* buff, size_t size) {
    assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip->memory.data[CHIP8_PROGRAM_LOAD_ADDRESS], buff, size);
    chip->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

static void chip8_exec_extended_8(chip8* chip, uint8_t x, uint8_t y, uint8_t n) {
    uint16_t temp = 0;
    switch (n) {
        /* 8xy0 - LD Vx, Vy: Set Vx = Vy */
        case 0x00:
            chip->registers.registers[x] = chip->registers.registers[y];
            break;
        /* 8xy1 - OR Vx, Vy: Set Vx = Vx OR Vy */
        case 0x01:
            chip->registers.registers[x] |= chip->registers.registers[y];
            break;

        /* 8xy2 - AND Vx, Vy: Set Vx = Vx AND Vy */
        case 0x02:
            chip->registers.registers[x] &= chip->registers.registers[y];
            break;

        /* 8xy3 - XOR Vx, Vy: Set Vx = Vx XOR Vy */
        case 0x03:
            chip->registers.registers[x] ^= chip->registers.registers[y];
            break;

        /* ADD Vx, Vy: Set Vx = Vx + Vy, set VF = carry */
        case 0x04:
            temp = chip->registers.registers[x] + chip->registers.registers[y];
            chip->registers.registers[0x0f] = temp > 0xff;
            chip->registers.registers[x]    = temp;
            break;

        /* SUB Vx, Vy: Set Vx = Vx - Vy, set VF = NOT borrow */
        case 0x05:
            chip->registers.registers[0x0f] =
                chip->registers.registers[x] > chip->registers.registers[y];
            chip->registers.registers[x] -= chip->registers.registers[y];
            break;

        /* SHR Vx {, Vy} */
        case 0x06:
            chip->registers.registers[0x0f] = chip->registers.registers[x] & 0b00000001;
            chip->registers.registers[x] /= 2;
            break;

        /* 8xy7 - SUBN Vx, Vy */
        case 0x07:
            chip->registers.registers[0x0f] =
                chip->registers.registers[x] < chip->registers.registers[y];
            chip->registers.registers[x] =
                chip->registers.registers[y] - chip->registers.registers[x];
            break;

        /* SHL Vx {, Vy} */
        case 0x0E:
            chip->registers.registers[0x0f] = chip->registers.registers[x] & 0b10000000;
            chip->registers.registers[x] *= 2;
            break;
    }
}

static int chip8_wait_for_key_press() {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type != SDL_KEYDOWN)
            continue;

        SDL_Keycode sdl_key   = event.key.keysym.sym;
        int         chip8_key = chip8_keyboard_map(sdl_key);
        if (chip8_key != -1) {
            return chip8_key;
        }
    }
    return -1;
}

static void chip8_exec_extended_F(chip8* chip, uint8_t x, uint8_t kk) {
    switch (kk) {
        /* LD Vx, DT */
        case 0x07:
            chip->registers.registers[x] = chip->registers.delay_timer;
            break;

        /* LD Vx, K */
        case 0x0A: {
            int key = chip8_wait_for_key_press();
            if (key != -1) {
                chip->registers.registers[x] = key;
                printf("Key pressed: 0x%02X\n", key);
            }
        } break;

        /* LD DT, Vx */
        case 0x15:
            chip->registers.delay_timer = chip->registers.registers[x];
            break;

        /* LD ST, Vx */
        case 0x18:
            chip->registers.sound_timer = chip->registers.registers[x];
            break;

        /* Add I, Vx */
        case 0x1E:
            chip->registers.I += chip->registers.registers[x];
            break;

        /* LD, F, Vx */
        case 0x29:
            chip->registers.I = chip->registers.registers[x] * CHIP8_SPRITE_HEIGHT;
            break;

        /* LD B, Vx */
        case 0x33: {
            uint8_t value = chip->registers.registers[x];
            chip8_memory_set(chip, chip->registers.I, value / 100);
            chip8_memory_set(chip, chip->registers.I + 1, (value / 10) % 10);
            chip8_memory_set(chip, chip->registers.I + 2, value % 10);
        } break;

        /* LD [I], Vx */
        case 0x55:
            for (int i = 0; i <= x; ++i) {
                chip8_memory_set(chip, chip->registers.I + i, chip->registers.registers[i]);
            }
            break;

        /* LD Vx, [I] */
        case 0x65:
            for (int i = 0; i <= x; ++i) {
                chip->registers.registers[i] = chip8_memory_get(chip, chip->registers.I + i);
            }
            break;
    }
}

static void chip8_exec_extended(chip8* chip, uint16_t opcode) {
    uint16_t nnn = opcode & 0x0fff;
    uint8_t  n   = opcode & 0x000f;
    uint8_t  x   = (opcode & 0x0f00) >> 8;
    uint8_t  y   = (opcode & 0x00f0) >> 4;
    uint8_t  kk  = opcode & 0x00ff;

    switch (opcode & 0xF000) {
        /* JP addr: Jump to location nnn */
        case 0x1000:
            chip->registers.PC = nnn;
            break;

        /* CALL addr: Call subroutine at nnn */
        case 0x2000:
            chip8_stack_push(chip, chip->registers.PC);
            chip->registers.PC = nnn;
            break;

        /* SE Vx, byte, Skip next instruction if Vx = kk */
        case 0x3000:
            if (chip->registers.registers[x] == kk) {
                chip->registers.PC += 2;
            }
            break;

        /* SNE Vx, byte: Skip next instruction if Vx != kk */
        case 0x4000:
            if (chip->registers.registers[x] != kk) {
                chip->registers.PC += 2;
            }
            break;

        /* 5xy0 - SE Vx, Vy: Skip next instruction if Vx = Vy */
        case 0x5000:
            if (chip->registers.registers[x] == chip->registers.registers[y]) {
                chip->registers.PC += 2;
            }
            break;

        /* 6xkk - LD Vx, byte: Set Vx = kk */
        case 0x6000:
            chip->registers.registers[x] = kk;
            break;

        /* 7xkk - ADD Vx, byte. Set Vx = Vx + kk */
        case 0x7000:
            chip->registers.registers[x] += kk;
            break;

        /* call extended 0x8 */
        case 0x8000:
            chip8_exec_extended_8(chip, x, y, n);
            break;

        /* SNE Vx, Vy: Skip next instruction if Vx != Vy */
        case 0x9000:
            if (chip->registers.registers[x] != chip->registers.registers[y]) {
                chip->registers.PC += 2;
            }
            break;

        /* LD I, addr: Set I = nnn */
        case 0xA000:
            chip->registers.I = nnn;
            break;

        /* JP V0, addr: Jump to location nnn + V0 */
        case 0xB000:
            chip->registers.PC = nnn + chip->registers.registers[0];
            break;

        /* RND Vx, byte: Set Vx = random byte AND kk */
        case 0xC000:
            srand(time(NULL));
            chip->registers.registers[x] = (rand() % 256) & kk;
            break;

        /* DRW Vx, Vy, nibble:
         * Display n-byte sprite starting at memory location I at (Vx, Vy), set
         * VF = collision
         */
        case 0xD000:
            chip8_screen_draw_sprite(chip,
                                     chip->registers.registers[x],
                                     chip->registers.registers[y],
                                     &chip->memory.data[chip->registers.I],
                                     n);
            break;

        case 0xE000:
            switch (opcode & 0x00FF) {
                /* SKP Vx */
                case 0x9E:
                    if (chip8_keyboard_is_down(chip, chip->registers.registers[x])) {
                        chip->registers.PC += 2;
                    }
                    break;

                /* SKPNP Vx */
                case 0xA1:
                    if (!chip8_keyboard_is_down(chip, chip->registers.registers[x])) {
                        chip->registers.PC += 2;
                    }
                    break;
            }
            break;

        case 0xF000:
            chip8_exec_extended_F(chip, x, kk);
            break;
    }
}

void chip8_exec(chip8* chip, uint16_t opcode) {
    switch (opcode) {
        case 0x00E0: /* CLS: clear the display*/
            chip8_screen_clear(chip);
            break;

        case 0x00EE: /* Ret: Return from a subroutine */
            chip->registers.PC = chip8_stack_pop(chip);
            break;

        default:
            chip8_exec_extended(chip, opcode);
    }
}
