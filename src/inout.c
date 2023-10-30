#include "inout.h"
#include "SDL2/SDL.h"
/*#include <stdint.h>
#include <stdio.h>*/

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 256

SDL_Window* screen;
SDL_Renderer* renderer;

void init_display(void){
	SDL_Init(SDL_INIT_VIDEO);

	screen = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
}

void draw(uint8_t* display){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//TODO
}


void stop_display(void) {
    SDL_DestroyWindow(screen);
    SDL_Quit();
}

int findFileSize(char filename[]) {
    FILE *chkExist = fopen(filename, "r");
    if (chkExist == NULL) {
        printf("File Not Found!\n");
        return -1;
    }

    fseek(chkExist, 0L, SEEK_END);

    // calculating the size of the file
    long int res = ftell(chkExist);

    // closing the file
    fclose(chkExist);

    return res;
}