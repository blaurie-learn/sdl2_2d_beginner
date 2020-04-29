#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//scroll speed in pixels per second
#define SCROLL_SPEED 5

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

	//struct to hold the position and size of the sprite
	SDL_Rect dest;

	//get the dimensions of the texture
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
	//scale the dimensions of the texture
	dest.w /= 4;
	dest.h /= 4;

	//start the sprite in the center of the screen
	float x_pos = (WINDOW_WIDTH - dest.w) / 2;
	float y_pos = (WINDOW_HEIGHT - dest.h) / 2;
	float x_vel = 0;
	float y_vel = 0;

	//keep track of which inputs were given:
	int up = 0;
	int down = 0;
	int left = 0;
	int right = 0;

	//set to 1 when window close button in pressed
	int close_requested = 0;

	//animation loop
	while (!close_requested) {
		//process events
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					close_requested = 1;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
						case SDL_SCANCODE_W:
						case SDL_SCANCODE_UP:
							up = 1;
							break;

						case SDL_SCANCODE_A:
						case SDL_SCANCODE_LEFT:
							left = 1;
							break;

						case SDL_SCANCODE_S:
						case SDL_SCANCODE_DOWN:
							down = 1;
							break;

						case SDL_SCANCODE_D:
						case SDL_SCANCODE_RIGHT:
							right = 1;
							break;

						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.scancode) {
						case SDL_SCANCODE_W:
						case SDL_SCANCODE_UP:
							up = 0;
							break;

						case SDL_SCANCODE_A:
						case SDL_SCANCODE_LEFT:
							left = 0;
							break;

						case SDL_SCANCODE_S:
						case SDL_SCANCODE_DOWN:
							down = 0;
							break;

						case SDL_SCANCODE_D:
						case SDL_SCANCODE_RIGHT:
							right = 0;
							break;

						default:
							break;
					}
					break;
			}
		}

		/**
		 * Mouse input stuff
		 */

		//get cursor position relative to window
		int mouse_x, mouse_y;
		int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

		// determine velocity toward mouse
		int target_x = mouse_x - dest.w / 2;
		int target_y = mouse_y - dest.h / 2;
		float delta_x = target_x - x_pos;
		float delta_y = target_y - y_pos;
		float distance = sqrt(delta_x * delta_x + delta_y * delta_y);

		//prevents jitter
		if (distance < 5) {
			x_vel = y_vel = 0;
		} else {
			x_vel = delta_x * SCROLL_SPEED / distance;
			y_vel = delta_y * SCROLL_SPEED / distance;
		}

		//if mouse button one is pressed, reverse direction
		if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			x_vel = -x_vel;
			y_vel = -y_vel;
		}

		/**
		 * end mouse input stuff
		 */

		if (x_vel == 0 && y_vel == 0) {
			if (up) {
				y_vel = -SCROLL_SPEED;
			}
			if (left) {
				x_vel = -SCROLL_SPEED;
			}
			if (down) {
				y_vel = SCROLL_SPEED;
			}
			if (right) {
				x_vel = SCROLL_SPEED;
			}
		}


		x_pos += x_vel;
		y_pos += y_vel;
		//collision detection with bounds
		if (x_pos <= 0) {
			x_pos = 0;
			x_vel = -x_vel;
		}
		if (y_pos <= 0) {
			y_pos = 0;
			y_vel = -y_vel;
		}
		if (x_pos >= WINDOW_WIDTH - dest.w) {
			x_pos = WINDOW_WIDTH - dest.w;
			x_vel = -x_vel;
		}
		if (y_pos >= WINDOW_HEIGHT - dest.h) {
			y_pos = WINDOW_HEIGHT - dest.h;
			y_vel = -y_vel;
		}
		dest.x = x_pos;
		dest.y = y_pos;

		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, tex, NULL, &dest);
		SDL_RenderPresent(rend);

		SDL_Delay(1000 / 60);

	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
