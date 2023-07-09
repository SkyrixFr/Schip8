#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t fontset[80] = {
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

uint8_t memory[4096] = {0};

uint8_t V[16] = {0};

uint8_t delayTimer = 0;
uint8_t soundTimer = 0;

uint8_t display[64 * 32] = {0};

// Pseudo register (dont know what it is yet)
uint8_t sp = 0;

// Program counter
uint16_t pc = 0x200;

// 16 bit register
uint16_t I = 0;

uint16_t stack[16] = {0};

// I prefer storing the actual opcode
uint16_t opcode = 0;

// Init the keyboard but i wont emplement it yet...
uint16_t keyboard[16] = {0};

void chip8_init(void){

    puts("Loading fontset in CMemory");
    for(int i=0; i<=80;++i){
        memory[i] = fontset[i];
    }
    puts("Done!");
}

// Wrong need to fix
int load_rom(char *filename){
    long int romsize = 0;

    FILE* fp = fopen(filename, "rb");
    if(fp == NULL){
        fprintf(stderr, "Error opening the file");
        return -1;
    }

    //find size
    fseek(fp, 0L, SEEK_END);
    romsize = ftell(fp);

    rewind(fp);

    uint8_t *buffer=malloc(romsize+0x200);
    fread(buffer+0x200, romsize, 1, fp);


    fclose(fp);
    return 0;
}