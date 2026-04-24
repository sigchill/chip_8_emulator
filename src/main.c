#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "chip8.h"
#include "platform.h"

#define ERROR 1
#define SUCCESS 0
#define SCALE 10
#define WINDOW_W (GFX_W * SCALE)
#define WINDOW_H (GFX_H * SCALE)
#define ON_PIXEL 0xFFFFFFFFu
#define OFF_PIXEL 0x000000FFu

static void update_video_buffer(uint32_t *video, const Chip8 *c)
{
    for (int i = 0; i < GFX_W * GFX_H; i++) {
        video[i] = c->gfx[i] ? ON_PIXEL : OFF_PIXEL;
    }
}


int main(int argc, char **argv){
    if(argc !=2){
        fprintf(stderr,"Usage: %s <ROM file>\n",argv[0]);
        return ERROR;
    }

    Platform platform;
    if (!platform_init(&platform, "CHIP-8", WINDOW_W, WINDOW_H, GFX_W, GFX_H)) {
        fprintf(stderr, "Platform init error: %s\n", platform_error());
        return ERROR;
    }
    
    Chip8 c;
    chip8_init(&c);
    if(!chip8_load_rom(&c,argv[1])){
        fprintf(stderr,"Failed to load ROM: %s\n",argv[1]);
        platform_destroy(&platform);
        return ERROR;
    }

    uint32_t video[GFX_W * GFX_H];
    update_video_buffer(video, &c);
    platform_update(&platform, video, GFX_W * (int)sizeof(video[0]));

    bool running = true;

    const int cycles_per_frame = 10;
    uint32_t last_timer_tick = SDL_GetTicks();

    while(running){
        if (platform_process_input(c.keys)) {
            running = false;
        }

      // cpu cycle
        for (int i = 0; i < cycles_per_frame; i++) {
            chip8_cycle(&c);
        }

        // 60hz cycle
        uint32_t now = SDL_GetTicks();
        // Tick timers every ~16ms (60Hz ~= 16.666ms)
        if (now - last_timer_tick >= 16) {
            if (c.delay_timer > 0) c.delay_timer--;
            if (c.sound_timer > 0) c.sound_timer--; // later: beep if >0
            last_timer_tick += 16; // keep it steady
        }

        // ---- Render (only when draw_flag set) ----
        if (c.draw_flag) {
            update_video_buffer(video, &c);
            platform_update(&platform, video, GFX_W * (int)sizeof(video[0]));
            c.draw_flag = false;
        }

        // Small delay so we don't peg CPU at 100%
        SDL_Delay(1);
    }

    platform_destroy(&platform);
    return SUCCESS;
}
