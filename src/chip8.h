#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

extern uint8_t memory[4096];
extern uint8_t V[16];
extern uint8_t delayTimer;
extern uint8_t soundTimer;
extern uint8_t drawflag;
extern uint8_t screen[64 * 32];
extern uint8_t sp; //stack-pointer
extern uint16_t pc;
extern uint16_t I;
extern uint16_t stack[16];
extern uint16_t opcode;
extern uint16_t keyboard[16];

void chip8_init();
int load_rom(char *filename);


#endif