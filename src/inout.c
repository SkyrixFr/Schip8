#include "inout.h"
#include "chip8.h"
#include "SDL2/SDL.h"

#include <string.h>

#include <unistd.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512

SDL_Window* debug_window;
SDL_Renderer* debug_renderer;
SDL_Window* window;
SDL_Renderer* renderer;

uint8_t quit = 0;

int init_display(void){
	SDL_Init(SDL_INIT_VIDEO);

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Erreur d'initialisation de la SDL : %s",SDL_GetError());
		return 1;
	}

	debug_window = SDL_CreateWindow("DEBUG", 1472, SDL_WINDOWPOS_CENTERED, 200, 512, 0);
	debug_renderer = SDL_CreateRenderer(debug_window, -1, SDL_RENDERER_ACCELERATED);

	window = SDL_CreateWindow("SChip8 Made by Skyrix_", SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	return 0;
}

void draw(uint8_t* screen){
	SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 64, 255, 0, 255);

	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			if (screen[y * 64 + x] == 1) {
				// Draw an 16x16 pixel
				SDL_Rect pixelRect = { x * 16, y * 16, 16, 16 };
				SDL_RenderFillRect(renderer, &pixelRect);
			}
		}
	}

	SDL_RenderPresent(renderer);
}


void handle_event(void) {
	SDL_Event event;

	// Adjust depending on your keyboard (mine's azerty, french)
	const SDL_Keycode keyMappings[16] = {
		SDLK_1, SDLK_2, SDLK_3, SDLK_4,
		SDLK_a, SDLK_z, SDLK_e, SDLK_r,
		SDLK_q, SDLK_s, SDLK_d, SDLK_f,
		SDLK_w, SDLK_x, SDLK_c, SDLK_v
	};

	while(SDL_PollEvent(&event)) {
		if(event.key.keysym.sym == SDLK_ESCAPE) {
			quit = 1;
		}

		else if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
			quit=1;
		}

		else if(event.key.keysym.sym == SDLK_p) {
			memset(V, 0, sizeof(V));
			delayTimer = 0;
			soundTimer = 0;
			memset(screen, 0, sizeof(screen));
			sp = 0;
			pc = 0x200;
			I = 0;
			memset(stack, 0, sizeof(stack));
			opcode = 0;
			chip8_init();
		}

		else if(event.type == SDL_KEYUP || event.type == SDL_KEYDOWN) {
			for (int i = 0; i < 16; ++i) {
				if(event.key.keysym.sym == keyMappings[i]) { 
					if(event.type == SDL_KEYUP) {
						keyboard[i] = 0;
					}
					if(event.type == SDL_KEYDOWN) {
						keyboard[i] = 1;
					}
				}
			}
		}

	}
}

void wait(uint32_t MS_PER_FRAME) {
	usleep(MS_PER_FRAME);
}

void stop_display(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(debug_renderer);
	SDL_DestroyWindow(debug_window);
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