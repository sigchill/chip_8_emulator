#include "platform.h"

#include <SDL2/SDL.h>
#include <string.h>

static int key_to_chip8(SDL_Keycode key)
{
    switch (key) {
        case SDLK_x: return 0x0;
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_q: return 0x4;
        case SDLK_w: return 0x5;
        case SDLK_e: return 0x6;
        case SDLK_a: return 0x7;
        case SDLK_s: return 0x8;
        case SDLK_d: return 0x9;
        case SDLK_z: return 0xA;
        case SDLK_c: return 0xB;
        case SDLK_4: return 0xC;
        case SDLK_r: return 0xD;
        case SDLK_f: return 0xE;
        case SDLK_v: return 0xF;
        default: return -1;
    }
}

bool platform_init(Platform *p, const char *title, int window_width, int window_height,
                   int texture_width, int texture_height)
{
    memset(p, 0, sizeof(*p));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        return false;
    }

    p->window = SDL_CreateWindow(title,
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 window_width,
                                 window_height,
                                 SDL_WINDOW_SHOWN);
    if (!p->window) {
        platform_destroy(p);
        return false;
    }

    p->renderer = SDL_CreateRenderer(p->window, -1,
                                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!p->renderer) {
        platform_destroy(p);
        return false;
    }

    p->texture = SDL_CreateTexture(p->renderer,
                                   SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   texture_width,
                                   texture_height);
    if (!p->texture) {
        platform_destroy(p);
        return false;
    }

    return true;
}

void platform_destroy(Platform *p)
{
    if (p->texture) {
        SDL_DestroyTexture(p->texture);
    }
    if (p->renderer) {
        SDL_DestroyRenderer(p->renderer);
    }
    if (p->window) {
        SDL_DestroyWindow(p->window);
    }

    memset(p, 0, sizeof(*p));
    SDL_Quit();
}

void platform_update(Platform *p, const void *buffer, int pitch)
{
    SDL_UpdateTexture(p->texture, NULL, buffer, pitch);
    SDL_RenderClear(p->renderer);
    SDL_RenderCopy(p->renderer, p->texture, NULL, NULL);
    SDL_RenderPresent(p->renderer);
}

bool platform_process_input(uint8_t *keys)
{
    bool quit = false;
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                    break;
                }

                {
                    int key = key_to_chip8(event.key.keysym.sym);
                    if (key != -1) {
                        keys[key] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                    }
                }
                break;

            default:
                break;
        }
    }

    return quit;
}

const char *platform_error(void)
{
    return SDL_GetError();
}
