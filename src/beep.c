#include "beep.h"

#include "SDL2/SDL.h"

#include <math.h>
#include <stddef.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    SDL_AudioDeviceID device;
    SDL_AudioSpec     spec;
    double            phase;
    double            frequency;
    bool              initialized;
    bool              playing;
} beep_state;

static beep_state state;

static void beep_audio_callback(void* userdata, Uint8* stream, int len) {
    (void)userdata;
    float* out     = (float*)stream;
    int    samples = len / (int)sizeof(float);

    if (!state.playing || state.device == 0) {
        SDL_memset(stream, 0, len);
        return;
    }

    double phase          = state.phase;
    const double increment = state.frequency / (double)state.spec.freq;

    for (int i = 0; i < samples; ++i) {
        phase += increment;
        if (phase >= 1.0)
            phase -= 1.0;
        out[i] = phase < 0.5 ? 0.2f : -0.2f;
    }

    state.phase = phase;
}

void beep_init(void) {
    if (state.initialized)
        return;

    SDL_AudioSpec desired;
    SDL_zero(desired);
    desired.freq     = 44100;
    desired.format   = AUDIO_F32SYS;
    desired.channels = 1;
    desired.samples  = 512;
    desired.callback = beep_audio_callback;

    state.device = SDL_OpenAudioDevice(NULL, 0, &desired, &state.spec, 0);
    if (state.device != 0) {
        SDL_PauseAudioDevice(state.device, 0);
    }

    state.phase       = 0.0;
    state.frequency   = 0.0;
    state.playing     = false;
    state.initialized = true;
}

void beep_start(int frequency) {
    if (!state.initialized)
        beep_init();
    if (state.device == 0)
        return;

    SDL_LockAudioDevice(state.device);
    state.frequency = (double)frequency;
    state.playing   = true;
    SDL_UnlockAudioDevice(state.device);
}

void beep_stop(void) {
    if (!state.initialized || state.device == 0)
        return;

    SDL_LockAudioDevice(state.device);
    state.playing = false;
    SDL_UnlockAudioDevice(state.device);
}

bool beep_is_playing(void) {
    return state.playing && state.device != 0;
}

void beep_shutdown(void) {
    if (!state.initialized)
        return;

    if (state.device != 0) {
        SDL_CloseAudioDevice(state.device);
        state.device = 0;
    }
    state.playing = false;
}
