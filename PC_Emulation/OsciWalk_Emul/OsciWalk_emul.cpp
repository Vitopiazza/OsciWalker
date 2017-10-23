#include <iostream>
#include <SDL.h>
#include "FreeRTOS.h"

extern "C" uint32_t* pixels;
extern "C" uint8_t last_event;

#define FRAME_TICKS 50

//************************
//*CODE RELEVANT FOR GAME*
//************************

//Events code
#define EV_NONE		0
#define EV_UP		1
#define EV_DOWN		2

//Graphics and game loop code
//#include "../../../Project/graphics.h"
//#include "../../../Project/game.h"

extern "C" void game_reset();

extern "C" void game_loop();

uint8_t get_event()
{
	return last_event;
}

int main(int argc, char ** argv)
{
    bool leftMouseButtonDown = false;
    bool quit = false;
    SDL_Event event;

	srand(1);

	uint32_t next_time = SDL_GetTicks();
	uint32_t current_time = SDL_GetTicks();

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("Osciwalker Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 512, 512);

    pixels = new uint32_t[512 * 512];
    //memset(pixels, 0, 512 * 512 * sizeof(Uint32));
	game_reset();

    while (!quit)
    {
        SDL_UpdateTexture(texture, NULL, pixels, 512 * sizeof(Uint32));

		memset(pixels, 0, 512 * 512 * sizeof(Uint32));
		last_event = 0;

		while(SDL_PollEvent(&event)){
			switch (event.type)
			{
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
					leftMouseButtonDown = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					leftMouseButtonDown = true;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
				case SDLK_UP:
						last_event = EV_UP;
					break;
				case SDLK_DOWN:
						last_event = EV_DOWN;
					break;
				}
                break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		game_loop();

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

		//Very crude frame rate limiter, might not work
		current_time = SDL_GetTicks();
		next_time += FRAME_TICKS;
		if(current_time > next_time){
			next_time = current_time;
		}else{
			SDL_Delay( next_time - current_time );
		}
    }

    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}