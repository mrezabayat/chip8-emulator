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
    fclose(rom);
    if (res != 1) {
        fprintf(stderr, "Failed to read from file, error code: %d\n", res);
        return EXIT_FAILURE;
    }

    chip8 chip;
    chip8_init(&chip);
    chip8_load(&chip, buff, (size_t)size);

    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Keep logical coordinates consistent across DPI scales
    SDL_RenderSetLogicalSize(
        renderer, CHIP8_WIDTH * CHIP8_WINDOW_SCALER, CHIP8_HEIGHT * CHIP8_WINDOW_SCALER);

    beep_init();

    // --- Timing setup ---
    const Uint32 TIMER_INTERVAL_MS = 1000 / 60; // CHIP-8 timers tick at 60 Hz
    Uint64       last_timer_tick   = SDL_GetTicks64();

    // How many opcodes to execute per video frame (~60 fps due to vsync)
    const int CYCLES_PER_FRAME = 8; // increase to go faster, decrease to go slower

    int running = 1;
    while (running) {
        // --- Events ---
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_KEYDOWN) {
                int key = chip8_keyboard_map(e.key.keysym.sym);
                if (key != -1)
                    chip8_keyboard_down(&chip, key);
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    running = 0;
            } else if (e.type == SDL_KEYUP) {
                int key = chip8_keyboard_map(e.key.keysym.sym);
                if (key != -1)
                    chip8_keyboard_up(&chip, key);
            }
        }

        // --- 60 Hz timers ---
        Uint64 now = SDL_GetTicks64();
        while (now - last_timer_tick >= TIMER_INTERVAL_MS) {
            if (chip.registers.delay_timer > 0) {
                --chip.registers.delay_timer;
            }

            if (chip.registers.sound_timer > 0 && !beep_is_playing()) {
                beep_start(CHIP8_BEEP_FREQUENCY);
            }

            if (chip.registers.sound_timer > 0) {
                --chip.registers.sound_timer;
                if (chip.registers.sound_timer == 0 && beep_is_playing()) {
                    beep_stop();
                }
            } else if (beep_is_playing()) {
                beep_stop();
            }

            last_timer_tick += TIMER_INTERVAL_MS;
        }

        // --- CPU: run multiple instructions per frame ---
        for (int i = 0; i < CYCLES_PER_FRAME; ++i) {
            uint16_t opcode = chip8_memory_get_two_bytes(&chip, chip.registers.PC);
            chip.registers.PC += 2;
            chip8_exec(&chip, opcode);
        }

        // --- Draw ---
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

        SDL_RenderPresent(renderer);
        // With vsync, this presents at the display refresh rate (typically ~60 Hz)
    }

    beep_stop();
    beep_shutdown();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
