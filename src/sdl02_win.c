#include <SDL2/SDL.h>
#include <stdio.h>

int main(void)
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
	{
		printf("error initializing: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello, SDL!",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			640, 480, 0);

	if (!win) {
		printf("error getting SDL window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Delay(5000);

	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
