#include "inout.h"
#include "SDL2/SDL.h"

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

void stop_display(void) {
    SDL_DestroyWindow(screen);
    SDL_Quit();
}