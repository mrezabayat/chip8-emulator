#include "SDL2/SDL.h"
#include "chip8.h"
#include <stdio.h>

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    chip8 chip;
    chip8_init(&chip);
    chip8_screen_pixel_set(&chip, 10, 10);

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

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 fps
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
