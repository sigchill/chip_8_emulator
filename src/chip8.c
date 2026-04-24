#include "chip8.h"
#include <stdio.h>
#include <string.h>



/*initial state of chip8,
load the font into 0x50
address starts at 0x200
*/
/* built-in font, with sprite data representing the hexadecimal numbers */
static const uint8_t chip8_font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
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
    c->draw_flag=false;
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
                //cls
                case 0x00E0: //disp_clear , clear gfx
                    memset(c->gfx,0,sizeof(c->gfx));
                    c->draw_flag=true;
                    break;
                    //ret 
                case 0x00EE:
                     //return , pc = stack[--sp] , like ret in assembly
                     if(c->sp>0){
                        c->sp--;
                        c->pc=c->stack[c->sp];
                     }
                    break;
                default:
                    break; //0NNN isnt needed for most roms so we will no implement it

            }
            break;
        //jmp
        case 0x1000: //goto NNN; , aka jump to NNN
            c->pc = nnn;
            break;
        //call
        case 0x2000: // call subroutine at NNNN
            if(c->sp<16){
                c->stack[c->sp] = c->pc;
                c->sp++;
                c->pc=nnn;
            }
            break;

        // SE Vx, byte - skip next instruction if Vx = kk
        case 0x3000:
                if(c->V[x] == kk) c->pc+=2;
            break;
        
        //SNE Vx, byte - skip next instruction if Vx != kk
        case 0x4000:
                 if(c->V[x] != kk) c->pc+=2;
            break;
        
        // SE Vx, Vy - skip next instruction if Vx = Vy
        case 0x5000: // if(VX=NN) conditional skip next instrctuion if vx==nn
            if(n == 0 && c->V[x] == c->V[y]) c->pc+=2;
            break;
       
        //LD Vx, byte - set Vx = kk
        case 0x6000: //set vx to kk
            c->V[x] = kk;
            break;
        
        //ADD Vx, byte - set Vx = Vx + kk
        case 0x7000:
            c->V[x] = (uint8_t)(c->V[x]+kk);
            break;
        
        //logical and arithmetic operations
        case 0x8000:
            switch(opcode & 0x000F){
                //LD Vx, Vy - set Vx = Vy
                case 0x0000: //set vx to vy
                    c->V[x] = c->V[y];
                    break;
                //OR Vx, Vy - set Vx = Vx OR Vy
                case 0x0001:
                    c->V[x] |= c->V[y];
                    break;
                
                case 0x0002: //AND Vx, Vy - set Vx = Vx AND Vy
                    c->V[x] &= c->V[y];
                    break;

                case 0x0003: //XOR Vx, Vy - set Vx = Vx XOR Vy
                    c->V[x] ^= c->V[y];
                    break;
                case 0x0004: //ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry
                    {
                        uint16_t sum = c->V[x] + c->V[y];
                        c->V[0xF] = (sum > 0xFF) ? 1 : 0; //set carry flag to 1 if bigger than 255
                        c->V[x] = (uint8_t)sum; //store the result in Vx
                    }
                    break;
                case 0x0005: //SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow
                    {
                        c->V[0xF] = (c->V[x] >= c->V[y]) ? 1 : 0; //set borrow flag to 0 if vx>=vy
                        c->V[x] = (uint8_t)(c->V[x] - c->V[y]);
                    }
                    break;
                case 0x0006: //SHR Vx, set Vx = SHR 1 ,if lsb of vx is 1 then vf set to 1
                    c->V[0xF] = c->V[x]&0x1u;
                    c->V[x]>>=1;
                    break;
                //SUBN Vx, Vy - set Vx = Vy - Vx, set VF = NOT borrow
                case 0x0007:
                    {
                        c->V[0xF]=(c->V[y]>=c->V[x])?1:0; //set borrow flag to 0 if vy>=vx
                        c->V[x] = (uint8_t)(c->V[y] - c->V[x]);
                    }
                    break;
                case 0x000E: //SHL Vx, set Vx = Vx SHL 1, VF is old MSB
                    c->V[0xF] = (c->V[x] & 0x80u) ? 1 : 0;
                    c->V[x] <<= 1;
                    break;
                default:
                    break;
            }
            break;

        // SNE Vx, Vy - skip next instruction if Vx != Vy
        case 0x9000:
            if(n == 0 && c->V[x] != c->V[y]) c->pc+=2;
            break;

        case 0xA000://draw(vx,vy,n)i
            c->I=nnn;
            break;
            
        case 0xD000:{ //DRW Vx, Vy, n
            uint8_t vx = c->V[x];
            uint8_t vy = c->V[y];

            c->V[0xF]=0;

            for(int row = 0; row<n; row++){
                uint8_t spriteb = c->memory[c->I+row];
                for(int col=0; col<8; col++){
                    if(spriteb&(0x80>>col)){
                        int px = (vx+col)%GFX_W;
                        int py = (vy+row)%GFX_H;
                        int idx = py * GFX_W + px;

                        if(c->gfx[idx]==1) c->V[0xF] =1;
                        c->gfx[idx]^=1;
                    }
                }
            }
                c->draw_flag=true;
                break;
        }
        case 0xF000:
            switch(kk){
                case 0x33: // BCD: store hundreds, tens, ones of Vx at I, I+1, I+2
                    c->memory[c->I] = c->V[x] / 100;
                    c->memory[c->I + 1] = (c->V[x] / 10) % 10;
                    c->memory[c->I + 2] = c->V[x] % 10;
                    break;
                case 0x55: // store V0 through Vx in memory starting at I
                    for(uint8_t i = 0; i <= x; i++){
                        c->memory[c->I + i] = c->V[i];
                    }
                    break;
                case 0x65: // load V0 through Vx from memory starting at I
                    for(uint8_t i = 0; i <= x; i++){
                        c->V[i] = c->memory[c->I + i];
                    }
                    break;
                default:
                    break;
            }
            break;
            default:
            break;
    }

}


//load rom into memory starting at 0x200
bool chip8_load_rom(Chip8 *c,const char *path){  
    // load rom into memory starting at 0x200
    //open the file in binary mode
    FILE *f = fopen(path,"rb");
    //check if file opened successfully
    if (!f) return false;
    //seek to end of file to determine its size
    fseek(f,0,SEEK_END);
    //get the size of the file
    long size = ftell(f);
    //seek back to beginning of file
    rewind(f);

    //check for overflows
    if (size <=0 || (STARTING_ADDRESS+size)>MAX_MEMORY){
        fclose(f);
        return false;
    }
    //read the file into memory starting at 0x200
    size_t nread=fread(&c->memory[STARTING_ADDRESS],1,(size_t)size,f);
    fclose(f);
    return nread == (size_t)size;
}
