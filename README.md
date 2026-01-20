# chip_8_emulator
## This is a chip-8 emulator in the building

about me : im interested in low level stuff, revrse engineering, pwn, systems in general
so to strengthen my programming skills i though i'd build a chip-8 emulaotr which is considered by most a very feasible feat for a single person


##CHIP-8 has the following components:

Memory: CHIP-8 has direct access to up to 4 kilobytes of RAM
Display: 64 x 32 pixels (or 128 x 64 for SUPER-CHIP) monochrome, ie. black or white
A program counter, often called just “PC”, which points at the current instruction in memory
One 16-bit index register called “I” which is used to point at locations in memory
A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
An 8-bit delay timer which is decremented at a rate of 60 Hz (60 times per second) until it reaches 0
An 8-bit sound timer which functions like the delay timer, but which also gives off a beeping sound as long as it’s not 0
16 8-bit (one byte) general-purpose variable registers numbered 0 through F hexadecimal, ie. 0 through 15 in decimal, called V0 through VF


resources i  use : http://devernay.free.fr/hacks/chip8/C8TECH10.HTM (the manual for commands)
                    https://tobiasvl.github.io/blog/write-a-chip-8-emulator/ (this is great its language agonstic you just need to read the logic, it's about chip-8 not how to build it)
