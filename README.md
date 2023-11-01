# Schip8 (not the SuperChip)

An emulator for the famous chip8.

To compile the program you need the SDL2 library

The makefile is just a boilerplate (couldn't be bothered to re-write one)

Compiled fine with:
- Windows 11 x64, Ryzen 5 3600, 16Gb of RAM
- SDL 2.28.4 (previously worked with 2.26.4)
- GCC 13.2.0 with MSYS2
- Make 4.4.1

This project and ALL of it files are licensed under the Buena Onda License Agreement (v1.1) (cf: COPYRIGHT.TXT)

To use the emulator/disassembler : prgName.exe rom.ch8

## TODO list

- [ ] Finish the keyboard implementation
- [ ] Debug Window (maybe gtk for better window, maybe with sliders, being able to change rom etc. ¯\\_(ツ)_/¯ )
- [ ] Audio implementation
- [ ] delay timer and sound timer
- [ ] The stack (really need to do that)
- [ ] Take care of Exxx and Fxxx
- [ ] Enable subroutines (links to the stack)
- [ ] A better handling of events (it's all messy)
- [ ] Maybe a fps counter (?)

---

## Cool things that i added for "conforting" UX

- Program adjusted for AZERTY keyboard (french) (DONE)
- The key P has been added to reset program (DONE)
- Making a debug window to present all of the register's values, the program counter etc. (TODO)