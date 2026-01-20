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
    
    
    
    /*An instruction is two bytes, 
    so we will need to read two successive bytes from memory and combine them into one 16-bit instruction
    we use bitwise opreation we move first intruction 8 bits left and then us OR to add 
    the next byte into the lower bits so we have a whole 16 bit command*/
    uint16_t opcode = (c->memory[c->pc]<<8) | c->memory[c->pc+1];
    c->pc+=2; //increate the program counter
    uint8_t  x  = (opcode & 0x0F00) >> 8;
    uint8_t  y  = (opcode & 0x00F0) >> 4;
    uint8_t  kk = (opcode & 0x00FF);
    uint16_t nnn = (opcode & 0x0FFF);
    uint8_t  n  = (opcode & 0x000F);
    /*decode and execute within each case*/
    /* for now we will implement the following:
    00E0 (clear screen)
1NNN (jump)
6XNN (set register VX)
7XNN (add value to register VX)
ANNN (set index register I)
DXYN (display/draw) , so we can test it with the ibm logo rom
    */
    switch (opcode & 0xF000){
        case 0x0000:
         //call , display , flow commands
            switch(opcode){
                case 0x00E0: //disp_clear , clear gfx
                    memset(c->gfx,0,sizeof(c->gfx));
                    break;
                case 0x00EE: //return , pc = stack[--sp] , like ret in assembly
                    break;
                default:
                    break; //0NNN isnt needed for most roms so we will no implement it

            }
            break;
        
        case 0x1000: //goto NNN; , aka jump to NNN
            c->pc = ((opcode&0x0FFF));
            break;
        
        case 0x2000: // call subroutine at NNNN
            break;

        case 0x3000 : // if(Vx!=NN) conditional skip next instruction if vx !=nn
            break;
        
        case 0x5000: // if(VX=NN) conditional skip next instrctuion if vx==nn
            break;
       
        case 0x6000: //set vx to nn
            x = (opcode&0x0F00)>>8;
            nn = (opcode&0x00FF);
            c->V[x] = nn;
            break;
        case 0x7000: //Vx+=nn
            x = (opcode&0x0F00)>>8;
            nn = (opcode&0x00FF);
            c->V[x] += nn;
            break; 
        case 0xA000: //I=NNN instruction cnt set to address NNN
            nn = (opcode&0x0FFF);
            c->I=nn;
            break;
        case 0xD000: // draw the display draw(vx,vy,N)
            x=(opcode&0x0F00)>>8;
            y=(opcode&0x00F0)>>4;
            n = (opcode&0x000F);

            



    }







}