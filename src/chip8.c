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
    int32_t romsize = 0;

    printf("%s\n",filename);

    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        fprintf(stderr, "Error opening the file");
        return -1;
    }

    //find size
    fseek(file, 0L, SEEK_END);
    romsize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    int8_t *buffer=malloc(romsize+1);
    int bufferSize = fread(buffer, sizeof(char), romsize, file);
    if(bufferSize!=romsize){
        fprintf(stderr, "Error of rom buffer\n");
        return -1;
    }

    fclose(file);

    if(romsize>=3584){
        fprintf(stderr, "Error, romsize too big!");
        return -1;
    }

    for (int32_t i = 0; i < romsize; ++i) {
        memory[0x200 + i] = buffer[i];
    }

    return 0;
}

void one_cycle(void){
    opcode = memory[pc] << 8 | memory[pc+1];
    //printf("pc %04x - %04x\n",pc-0x200,opcode);

    // configurations : YxyZ, Ynnn, Yxnn, Yx
    uint16_t x = (opcode & 0x0f00) >> 8;
    uint16_t y = (opcode & 0x00f0) >> 4;

    printf("%04X -> ",opcode);
    //check first nib
    switch (opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x00ff){
                case 0x00E0:
                    printf("%-10s %s\n", "CLS", "Clears screen");
                    break;

                case 0x00EE:
                    printf("%-10s %s\n", "RET", "Return from a subroutine");
                break;

                default:
                    printf("%-10s %s\n", "UNK", "Unknown opcode");

            }
        break;
        default:
            printf("%-10s %s\n", "UNK", "Unknown opcode");

    }


}