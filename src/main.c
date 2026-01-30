#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "chip8.h"
#define ERROR 1
#define GFX_W 64
#define GFX_H 32
#define SUCCESS 0
#define SCALE 10
#define WINDOW_W (GFX_W * SCALE)
#define WINDOW_H (GFX_H * SCALE)

// Map SDL key to CHIP-8 key index (0x0..0xF). Return -1 if not mapped.
static int key_to_chip8(SDL_Keycode key) {
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

static void render_chip8(SDL_Renderer *ren , const Chip8 *c){
    SDL_SetRenderDrawColor(ren,0,0,0,255);
    //CLEAR screen to black color
    SDL_RenderClear(ren);

    //draw white pixels for gfx=1
    SDL_SetRenderDrawColor(ren,255,255,255,255);
    
    //iterate over gfx array
    for(int y=0; y<GFX_H; y++){
        for(int x=0; x<GFX_W; x++){
            if(c->gfx[y*GFX_W + x]){
                SDL_Rect r = {x*SCALE, y*SCALE, SCALE, SCALE};
                SDL_RenderFillRect(ren,&r);
            }
        }
    }
    SDL_RenderPresent(ren);
}


int main(int argc, char **argv){
    uint32_t last_timer_tick = SDL_GetTicks();
    SDL_Renderer *ren;

    if(argc !=2){
        fprintf(stderr,"Usage: %s <ROM file>\n",argv[0]);
        return ERROR;
    }

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)!=0){
        fprintf(stderr,"SDL_Init Error: %s\n",SDL_GetError());
        return ERROR;
    }
    SDL_Window *win = SDL_CreateWindow("CHIP-8",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WINDOW_W,WINDOW_H,SDL_WINDOW_SHOWN);
    if(!win){
        fprintf(stderr,"SDL_CreateWindow Error: %s\n",SDL_GetError());
        SDL_Quit();
        return ERROR;
    
    }
    ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!ren){
        fprintf(stderr,"SDL_CreateRenderer Error: %s\n",SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return ERROR;
    }
    
    Chip8 c;
    chip8_init(&c);
    if(!chip8_load_rom(&c,argv[1])){
        fprintf(stderr,"Failed to load ROM: %s\n",argv[1]);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return ERROR;
    }

    bool running = true;

    const int cycles_per_frame = 10;
    uint32_t frame_delay = SDL_GetTIcks();

    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                running = false;
            }else if(e.type == SDL_KEYDOWN){
                int key = key_to_chip8(e.key.keysym.sym);
                if(key != -1){
                    c.keys[key] =1;
                }
            }else if(e.type == SDL_KEYUP){
                int key = key_to_chip8(e.key.keysym.sym);
                if(key != -1){
                    c.keys[key] =0;
                }
            }
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
            render_chip8(ren, &c);
            c.draw_flag = false;
        }

        // Small delay so we don't peg CPU at 100%
        SDL_Delay(1);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return SUCCESS;
}