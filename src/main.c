#include "SDL2/SDL.h"
#include "beep.h"
#include "chip8.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <chip8-ROM>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    printf("Loading ROM: %s\n", filename);

    FILE* rom = fopen(filename, "rb");
    if (!rom) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return EXIT_FAILURE;
    }

    fseek(rom, 0, SEEK_END);
    long size = ftell(rom);
    printf("Size of ROM is: %ld\n", size);
    fseek(rom, 0, SEEK_SET);

    char buff[size];
    int  res = fread(buff, size, 1, rom);
    if (res != 1) {
        fprintf(stderr, "Failed to read from file, error code: %d\n", res);
        fclose(rom);
        return EXIT_FAILURE;
    }

    chip8 chip;
    chip8_init(&chip);
    chip8_load(&chip, buff, size);

    chip8_screen_draw_sprite(&chip, 0, 0, &chip.memory.data[0], 5);
    chip8_screen_draw_sprite(&chip, 5, 0, &chip.memory.data[5], 5);
    chip8_screen_draw_sprite(&chip, 10, 0, &chip.memory.data[10], 5);
    chip8_screen_draw_sprite(&chip, 15, 0, &chip.memory.data[15], 5);
    chip8_screen_draw_sprite(&chip, 20, 0, &chip.memory.data[20], 5);
    chip8_screen_draw_sprite(&chip, 25, 0, &chip.memory.data[25], 5);
    chip8_screen_draw_sprite(&chip, 30, 0, &chip.memory.data[30], 5);
    chip8_screen_draw_sprite(&chip, 35, 0, &chip.memory.data[35], 5);
    chip8_screen_draw_sprite(&chip, 40, 0, &chip.memory.data[40], 5);
    chip8_screen_draw_sprite(&chip, 45, 0, &chip.memory.data[45], 5);

    chip8_screen_draw_sprite(&chip, 0, 6, &chip.memory.data[50], 5);
    chip8_screen_draw_sprite(&chip, 5, 6, &chip.memory.data[55], 5);
    chip8_screen_draw_sprite(&chip, 10, 6, &chip.memory.data[60], 5);
    chip8_screen_draw_sprite(&chip, 15, 6, &chip.memory.data[65], 5);
    chip8_screen_draw_sprite(&chip, 20, 6, &chip.memory.data[70], 5);
    chip8_screen_draw_sprite(&chip, 25, 6, &chip.memory.data[75], 5);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(EMULATOR_WINDOW_TITLE,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          CHIP8_WIDTH * CHIP8_WINDOW_SCALER,
                                          CHIP8_HEIGHT * CHIP8_WINDOW_SCALER,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // keep logical coordinates consistent across DPI scales
    SDL_RenderSetLogicalSize(renderer, 640, 320);

    int running = 1;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = 0;
            if (e.type == SDL_KEYDOWN) {
                int key = chip8_keyboard_map(e.key.keysym.sym);
                if (key != -1) {
                    chip8_keyboard_down(&chip, key);
                }

                if (e.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
            }
            if (e.type == SDL_KEYUP) {
                int key = chip8_keyboard_map(e.key.keysym.sym);
                if (key != -1) {
                    chip8_keyboard_up(&chip, key);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int x = 0; x < CHIP8_WIDTH; ++x) {
            for (int y = 0; y < CHIP8_HEIGHT; ++y) {
                if (chip8_screen_is_pixel_set(&chip, x, y)) {
                    SDL_Rect rect = {.x = x * CHIP8_WINDOW_SCALER,
                                     .y = y * CHIP8_WINDOW_SCALER,
                                     .w = CHIP8_WINDOW_SCALER,
                                     .h = CHIP8_WINDOW_SCALER};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        while (chip.registers.delay_timer > 0) {
            chip.registers.delay_timer -= 6;
            SDL_Delay(CHIP8_DELAY_TIME_MS);
            printf("Value of delay_timer: %d\n", chip.registers.delay_timer);
        }

        while (chip.registers.sound_timer > 0) {
            chip.registers.sound_timer -= 6;
            beep(CHIP8_BEEP_FREQUENCY, CHIP8_DELAY_TIME_MS);
        }

        SDL_RenderPresent(renderer);
        // SDL_Delay(16); // ~60 fps
        int16_t opcode = chip8_memory_get_two_bytes(&chip, chip.registers.PC);
        chip8_exec(&chip, opcode);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
