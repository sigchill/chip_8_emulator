#include "chip8.h"


/*initial state of chip8,
load the font into 0x50
address starts at 0x200
*/
/* built-in font, with sprite data representing the hexadecimal numbers */
static const uint8_t chip8_font[80] = {  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(Chip8 *c ){
    memset(c,0,sizeof(*c)); //clear all
    c->pc=STARTING_ADDRESS; //0x200
    c->sp=0;
    c->I=0;
    c->delay_timer=0;
    c->sound_timer=0;
    chip8_load_font(c);  
}


// load the font into the memory of chip8
void chip8_load_font(Chip8 *c){
    for(int i =0; i < 80; i++){
        c->memory[FONT_START+i] = chip8_font[i];
    }
}

void chip8_cycle(Chip8 *c){

    /*fetch*/
    /*An instruction is two bytes, 
    so we will need to read two successive bytes from memory and combine them into one 16-bit instruction
    we use bitwise opreation we move first intruction 8 bits left and then us OR to add 
    the next byte into the lower bits so we have a whole 16 bit command*/
    uint16_t opcode = (c->memory[c->pc]<<8) | c->memory[c->pc+1];
    c->pc+=2; //increate the program counter

    /*decode*/
    






}