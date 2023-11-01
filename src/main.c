#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "chip8.h"
#include "inout.h"

#define TARGET_FPS 10
#define MS_PER_FRAME (300000/TARGET_FPS)

int main(int argc, char* argv[])
{
    /* Verifications*/
    if(argc!=2){
        fprintf(stderr, "[Error] command must be used like : emulator.exe rom.ch8\n");
        return 1;
    }

    if (init_display()){
        return 1;
    }

    chip8_init();
    char* filename = argv[1];
    int chkErrHand = load_rom(filename);
    if (chkErrHand != 0){
        fprintf(stderr,"Error! Nb : %d",chkErrHand);
        return 1;
    }

    while(pc<0xfff && !quit) {
        wait(MS_PER_FRAME);
        handle_event();
        one_cycle();
        draw(screen);
    }

    stop_display();
    return 0;
}