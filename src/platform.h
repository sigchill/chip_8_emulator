#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    struct SDL_Window *window;
    struct SDL_Renderer *renderer;
    struct SDL_Texture *texture;
} Platform;

bool platform_init(Platform *p, const char *title, int window_width, int window_height,
                   int texture_width, int texture_height);
void platform_destroy(Platform *p);
void platform_update(Platform *p, const void *buffer, int pitch);
bool platform_process_input(uint8_t *keys);
const char *platform_error(void);

#endif
