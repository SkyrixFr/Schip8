#include <stdio.h>
#include "chip8.h"
#include "inout.h"
#include "fileSize.h"

int main(int argc, char* argv[])
{
    /* Verifications*/
    if(argc!=2){
        fprintf(stderr, "[Error] command must be used like : emulator.exe rom.ch8\n");
        return -1;
    }

    init_display();

    stop_display();
    return 0;
}