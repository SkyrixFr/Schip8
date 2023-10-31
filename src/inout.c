#include "inout.h"
#include "SDL2/SDL.h"
/*#include <stdint.h>
#include <stdio.h>*/

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512

SDL_Window* window;
SDL_Renderer* renderer;

int init_display(void){
	SDL_Init(SDL_INIT_VIDEO);

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    	printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
    	return 1;
	}

	window = SDL_CreateWindow("SChip8 Made by Skyrix_", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	return 0;
}

void draw(uint8_t* screen){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set the background color (black)
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set the pixel color (white)

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (screen[y * 64 + x] == 1) {
                // Draw an 8x8 pixel square at (x * 8, y * 8)
                SDL_Rect pixelRect = { x * 8, y * 8, 8, 8 };
                SDL_RenderFillRect(renderer, &pixelRect);
            }
        }
    }

    SDL_RenderPresent(renderer); // Update the screen
}


void stop_display(void) {
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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