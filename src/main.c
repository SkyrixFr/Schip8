#include <stdio.h>
#include <stdlib.h>
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

    FILE *fp = fopen(argv[1], "rb");
    long filesize = findFileSize(argv[1]);
    printf("The file's size is: %ld\n", filesize);
    uint8_t *buffer=malloc(filesize+0x200);
    fread(buffer+0x200, filesize, 1, fp);
    fclose(fp);

    init_display();

    chip8_init();
    load_rom();

    stop_display();
    return 0;
}