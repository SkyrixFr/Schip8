#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chip8.h"
#include "inout.h"

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

uint8_t screen[64 * 32] = {0};

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
    for(int i=0; i<80;++i){
        memory[i] = fontset[i];
    }
    puts("Done!");

    puts("Init srand()");
    srand((uint32_t)time(NULL));
    puts("Done!");

    puts("Finished Init");
}

// Wrong need to fix
int load_rom(char *filename){
    int32_t romsize = 0;

    printf("%s\n",filename);

    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        fprintf(stderr, "Error opening the file\n");
        fclose(file);
        return 1;
    }

    //find size
    fseek(file, 0L, SEEK_END);
    romsize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    if(romsize>=3584){
        fprintf(stderr, "Error, romsize too big!\n");
        fclose(file);
        return 1;
    }

    int8_t *buffer=malloc(romsize+1);
    int bufferSize = fread(buffer, sizeof(char), romsize, file);
    if(bufferSize!=romsize){
        fprintf(stderr, "Error of rom buffer\n");
        fclose(file);
        return 1;
    }

    fclose(file);

    for (int32_t i = 0; i < romsize; ++i) {
        memory[0x200 + i] = buffer[i];
    }

    free(buffer);
    return 0;
}

void print_display(uint8_t* screen) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            fprintf(stderr,"%d ", screen[y * 64 + x]);
        }
        fprintf(stderr,"\n"); // Move to the next row
    }
}

void one_cycle(void){
    opcode = memory[pc] << 8 | memory[pc+1];

    // configurations : YxyZ, Ynnn, Yxnn, Yx
    uint16_t x = (opcode & 0x0f00) >> 8;
    uint16_t y = (opcode & 0x00f0) >> 4;
    uint16_t nn = (opcode & 0x00ff);
    uint16_t nnn = (opcode & 0x0fff);

    fprintf(stderr,"pc 0x%03x -> %04X -> ",pc,opcode);
    //check first nib
    switch (opcode & 0xF000){
        case 0x0000:
            switch(nn){
                case 0xE0:
                    fprintf(stderr,"%-10s %s\n", "CLS", "Clears screen");
                    for(int i=0;i<64*32;++i){screen[i]=0;}
                    pc+=2;
                    break;

                case 0xEE:
                    fprintf(stderr,"%-10s %s\n", "RET", "Return from a subroutine");
                    // don't know what it is need to do some research

                    pc+=2;
                    break;

                default:
                    fprintf(stderr,"%-10s %s\n", "UNK", "Unknown opcode");
                    pc+=2;
                    break;

            }
            break;
        case 0x1000:
            fprintf(stderr,"%-10s %s %x\n", "JMP", "Jump to:",nnn);
            pc=nnn;
            break;
        case 0x2000:
            fprintf(stderr, "%-10s %s\n", "NIY", "2000 NOT IMPLEMENTED YET");
            pc+=2;
            break;
        case 0x3000:
            fprintf(stderr, "%-10s %s%x %s%x %s\n", "SKIE", "Skip next instruction if V",x,"== 0x",nn,V[x]==nn?"TRUE":"FALSE");
            if(V[x]==nn){
                pc+=2;
            }
            pc+=2;
            break;
        case 0x4000:
            fprintf(stderr, "%-10s %s%x %s%x %s\n", "SKINE", "Skip next instruction if V",x,"!= 0x",nn,V[x]!=nn?"TRUE":"FALSE");
            if(V[x]!=nn){
                pc+=2;
            }
            pc+=2;
            break;
        case 0x5000:
            fprintf(stderr, "%-10s %s%x %s%x %s\n", "SKIVE", "Skip next instruction if V",x,"== V",y,V[x]==V[y]?"TRUE":"FALSE");
            if(V[x]==V[y]){
                pc+=2;
            }
            pc+=2;
            break;
        case 0x6000:
            fprintf(stderr, "%-10s %s%x %s%x\n", "SET", "Set V",x,"to 0x",nn);

            V[x]=nn;
            pc+=2;
            break;
        case 0x7000:
            fprintf(stderr, "%-10s %s%x %s%x\n", "ADDVNN", "Add 0x",nn,"to V",x);
            V[x]+=nn;
            pc+=2;
            break;
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0:
                    fprintf(stderr, "%-10s %s%x %s%x\n", "SETXY", "Set V",x,"to V",y);
                    V[x]=V[y];
                    pc+=2;
                    break;
                case 0x1:
                    //Vx = Vx or Vy
                    fprintf(stderr, "%-10s %s%x %s%x %s%x\n", "ORXY", "Set V",x,"to V",x,"| V",y);
                    V[x] = (V[x] | V[y]);
                    pc+=2;
                    break;
                case 0x2:
                    //Vx = Vx and Vy
                    fprintf(stderr, "%-10s %s%x %s%x %s%x\n", "ANDXY", "Set V",x,"to V",x,"& V",y);
                    V[x] = (V[x] & V[y]);
                    pc+=2;
                    break;
                case 0x3:
                    //Vx = Vx xor Vy
                    fprintf(stderr, "%-10s %s%x %s%x %s%x\n", "XORXY", "Set V",x,"to V",x,"^ V",y);
                    V[x] = (V[x] ^ V[y]);
                    pc+=2;
                    break;
                case 0x4:
                    fprintf(stderr, "%-10s %s%x %s%x %s\n", "ADDXY", "ADD V",y,"to V",x,"with carry in Vf (set to 1 or 0)");
                    V[0xF] = (V[x] + V[y] > 0xFF) ? 1 : 0;
                    V[x] += V[y];
                    pc+=2;
                    break;
                case 0x5:
                    fprintf(stderr, "%-10s %s%x %s%x %s\n", "SUBXY", "SUB V",y,"to V",x,"with borrow in Vf (set to 1 or 0)");
                    V[0xF] = (V[x] > V[y]) ? 1 : 0;
                    V[x] -= V[y];
                    pc+=2;
                    break;
                case 0x6:
                    fprintf(stderr, "%-10s %s%x %s\n", "SHR", "VF = least sig then V",x,"is shifted by 1 to the right");
                    pc+=2;
                    break;
                //TODO the rest
                case 0x7:
                    //...
                    pc+=2;
                    break;
                case 0xE:
                    //...
                    pc+=2;
                    break;
                default:
                    fprintf(stderr, "Error, %X does not exist\n", opcode);
                    pc+=2;
                    break;
            }
        break;
        case 0x9000:
            fprintf(stderr, "%-10s %s%x %s%x %s\n", "SKIVNE", "Skip next instruction if V",x,"!= V",y,V[x]!=V[y]?"TRUE":"FALSE");
            if(V[x]!=V[y]){
                pc+=2;
            }
            pc+=2;
            break;
        case 0xA000:
            fprintf(stderr, "%-10s %s%x\n", "SETI", "Set I to ", nnn);
            I=nnn;
            pc+=2;
            break;
        case 0xB000:
            fprintf(stderr, "%-10s %s%x %s\n", "JMP+V0", "Jump to address ",nnn," + V0");
            pc=V[0]+nnn;
            break;
        case 0xC000:
            fprintf(stderr, "%-10s %s%x %s%x\n", "VERAND", "V",x,"is equal to random number & ",nn);
            V[x] = (rand() % 256) & nn;
            pc+=2;
            break;
        case 0xD000:
            fprintf(stderr, "%-10s %s%x%s%x %s\n", "DRW", "Draw to the screen at coordinates V", x, " and V",y ,"It will");
            fprintf(stderr, "I -> %x\n", I);
            fprintf(stderr, "x -> %x / V%x -> %x\n", x, x, V[x]);
            fprintf(stderr, "y -> %x / V%x -> %x\n", y, y, V[y]);

            uint16_t height = opcode & 0x000F; // x and y already defined

            V[0xF] = 0; // Initialize collision flag to 0
            for (int row = 0; row < height; row++) {
                uint8_t spriteByte = memory[I + row];
                for (int col = 0; col < 8; col++) {
                    if ((spriteByte & (0x80 >> col)) != 0) {
                        // Check if the pixel is set
                        int index = (V[y] + row) * 64 + (V[x] + col);
                        if (index < 64 * 32) {
                            if (screen[index] == 1) {
                                V[0xF] = 1;
                            }
                            // XOR the pixel to toggle its state
                            screen[index] ^= 1;
                        }
                    }
                }
            }
            pc += 2;
            break;
        case 0xE000:
            switch(opcode & 0x00ff){
                case 0x009e:
                    break;
                case 0x00a1:
                    break;
                default:
                    printf("%-10s %s\n", "UNK", "Unknown opcode");
                    pc+=2;
                    break;
            }
            break;
        case 0xF000:
            switch(opcode & 0x00ff){
                case 0x0007:
                    break;
                default:
                    printf("%X Does not exit\n",opcode);
                    pc+=2;
                    break;
            }
            break;
        default:
            printf("%-10s %s\n", "UNK", "Unknown opcode");
            pc+=2;
            break;

    }


}