#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(void)
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello, SDL!",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			640, 480, 0);

	if (!win) {
		printf("error creating window: %s\n", SDL_GetError());
		return 1;
	}

	//create a renderer which will set up the graphics hardware
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
	if (!rend) {
		printf("error creating renderer: %s", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	//load the image into memory using SDL_image library function
	SDL_Surface *surface = IMG_Load("resources/hello.png");
	if (!surface) {
		printf("error creating surface: %s", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);
	if (!tex) {
		printf("error creating texture: %s", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_DestroyTexture(tex);

	//clear the window
	SDL_RenderClear(rend);

	//draw the image to the window
	SDL_RenderCopy(rend, tex, NULL, NULL);
	SDL_RenderPresent(rend);

	//wait a few seconds
	SDL_Delay(5000);

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
