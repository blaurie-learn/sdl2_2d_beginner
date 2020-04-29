#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//scroll speed in pixels per second
#define SCROLL_SPEED 300

int main(void)
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello, SDL!",
									   SDL_WINDOWPOS_CENTERED,
									   SDL_WINDOWPOS_CENTERED,
									   WINDOW_WIDTH, WINDOW_HEIGHT, 0);

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

	/**
	 * Admination stuff starts
	 */
	//struct to hold the position and size of the sprite
	SDL_Rect dest;

	//get the dimensions of the texture
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

	//position the sprite at the bottom center of the window.
	//origin is the top left corner, positive y is down
	dest.x = (WINDOW_WIDTH - dest.w) / 2;

	float y_pos = WINDOW_HEIGHT;

	//animation loop
	while (dest.y >= -dest.h) {
		//clear the window
		SDL_RenderClear(rend);

		dest.y = (int) y_pos;

		//draw the image to the window
		SDL_RenderCopy(rend, tex, NULL, &dest);
		SDL_RenderPresent(rend);

		//update the sprite position
		y_pos -= (float) SCROLL_SPEED / 60;

		//wait 1/60 of a second
		SDL_Delay(1000/60);
	}


	/**
	 * Animation stuff ends
	 */

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
