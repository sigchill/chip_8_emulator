# CHIP-8 Emulator

A fully functional CHIP-8 emulator written in C, capable of running classic ROMs including Pong, Tetris, and Space Invaders.

> Built to deepen my understanding of low-level systems, CPU emulation, and memory-mapped I/O.

---

## Features

- Full implementation of the 35 CHIP-8 opcodes
- 64x32 monochrome display rendering
- 4KB memory with proper font sprite loading
- 16 general-purpose 8-bit registers (V0–VF)
- Delay and sound timers running at 60Hz
- Stack-based subroutine calls (16 levels deep)
- Keyboard input mapping (16-key hex keypad)

---

## Build & Run

### Requirements
- GCC or Clang
- SDL2 (`sudo apt install libsdl2-dev` on Ubuntu)

### Build
```bash
git clone https://github.com/sigchill/chip_8_emulator.git
cd chip_8_emulator
make
```

### Run
```bash
./chip8 <path-to-rom>
```

### Example
```bash
./chip8 roms/pong.ch8
```

---

## Architecture

| Component | Implementation |
|---|---|
| Memory | 4KB RAM array, programs loaded at 0x200 |
| Display | 64x32 pixel buffer, rendered via SDL2 |
| Registers | 16 x 8-bit (V0–VF) + 16-bit Index (I) |
| Stack | 16-level address stack for subroutines |
| Timers | Delay + sound timers decremented at 60Hz |
| Input | SDL2 keyboard mapped to 16-key hex pad |

---

## Opcode Coverage

All 35 standard CHIP-8 opcodes implemented, including:

- `00E0` — Clear display
- `1NNN` — Jump to address
- `6XNN` — Set register VX
- `DXYN` — Draw sprite (XOR-based, sets VF on collision)
- `FX33` — BCD representation
- `FX55/FX65` — Register store/load to memory

---

## What I Learned

- How a CPU fetch-decode-execute cycle works at the implementation level
- Memory-mapped I/O and how display buffers work
- Bitwise operations for opcode decoding (masking, shifting)
- How timers and interrupts are approximated in software
- Debugging with test ROMs to isolate opcode failures

---

## Resources

- [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
- [tobiasvl's Guide (language-agnostic)](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)

---

## Author

**sigchill** — interested in low-level systems, reverse engineering, and systems programming.
