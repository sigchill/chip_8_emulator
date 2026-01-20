//chip-8 header



#include <stdint.h>
#include <stdbool.h>

#define MAX_MEMORY 4096
#define GFX_W 64
#define GFX_H 32
#define STACK_SIZE 16
#define REGISTERS_AMOUNT 16
#define KEYS_AMOUNT 16
#define FONT_START 0x50
#define STARTING_ADDRESS 0x200

/*
Memory: CHIP-8 has direct access to up to 4 kilobytes of RAM
Display: 64 x 32 pixels (or 128 x 64 for SUPER-CHIP) monochrome, ie. black or white
A program counter, often called just “PC”, which points at the current instruction in memory
One 16-bit index register called “I” which is used to point at locations in memory
A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
An 8-bit delay timer which is decremented at a rate of 60 Hz (60 times per second) until it reaches 0
An 8-bit sound timer which functions like the delay timer, but which also gives off a beeping sound as long as it’s not 0
16 8-bit (one byte) general-purpose variable registers numbered 0 through F hexadecimal, ie. 0 through 15 in decimal, called V0 through VF
*/

typedef struct{
    uint8_t memory[MAX_MEMORY];
    uint16_t pc; //prgrm cnt
    uint16_t I; // index 
    uint8_t sp; //stack points
    uint8_t keys[KEYS_AMOUNT]; //keypad keys EX9E...etc
    uint16_t stack[STACK_SIZE];
    uint8_t delay_timer; //decrements at rate of 60hz until it reaches 0
    uint8_t sound_timer; //beeps as long as its not 0
    uint8_t V[REGISTERS_AMOUNT]; //16 general purpose registers called V0-VF
    uint8_t gfx[GFX_W*GFX_H]; //display
    bool draw_flag;
}Chip8;







/*inital state of chip8*/
void chip8_init(Chip8 *c);

/*load the font to memory of chip8*/
void chip8_load_font(Chip8 *c);
/*implements a chip8 cycle fetch-decode-execute*/
void chip8_cycle(Chip8 *c);
/*load a rom into the emulator*/
bool chip8_load_rom(Chip8 *c, const char *path);