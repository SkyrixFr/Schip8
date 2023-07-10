#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "chip8.h"
#include "inout.h"

//#include "fileSize.h"

int main(int argc, char* argv[])
{
    /* Verifications*/
    if(argc!=2){
        fprintf(stderr, "[Error] command must be used like : emulator.exe rom.ch8\n");
        return -1;
    }

    init_display();

    chip8_init();
    uint8_t* filename = argv[1];
    load_rom(filename);
    
    for(pc;pc<0xfff;pc+=0){
    one_cycle();
    }

    stop_display();
    return 0;
}