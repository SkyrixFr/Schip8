#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#define END_OF_MEMORY 0xfff

/* FROM EMULATOR101.COM
 * Display Buffer starts at memory 0xF00
 * 
 * 
 * 
 * 
 * 
 * 
 */

extern uint8_t memory[END_OF_MEMORY+1]; // memory of size 0xfff + 1
extern uint8_t V[16];					// 16 General purpose register V0 -> Vf
extern uint8_t delayTimer;				// dT -> wikipedia (not done yet)
extern uint8_t soundTimer;				// sT -> wikipedia (not done yet)
extern uint8_t drawflag;				// draw flag, saw this in another code, NB: yeah i used it
extern uint8_t screen[64 * 32];			// screen of 64 x 32 pixels
extern uint8_t sp; 						// stack-pointer
extern uint16_t pc;						// program counter
extern uint16_t I;						// 16 bit register (generally used for memory adresses)
extern uint16_t stack[16];				// the stack (not implemented yet but i should bc it is used in some programs)
extern uint16_t opcode;					// current opcode running
extern uint16_t keyboard[16];			// Keyboard (not implemented yet, not familiar yet with the SDL API)

void chip8_init();
int load_rom(char *filename);
void one_cycle();


#endif