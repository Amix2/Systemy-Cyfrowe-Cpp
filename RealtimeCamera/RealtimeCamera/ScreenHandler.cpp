#include "stdafx.h"
#include "ScreenHandler.h"

void initScreen(int W, int H, Screen* screen) {
	screen->window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, 0);
	screen->renderer = SDL_CreateRenderer(screen->window, -1, 0);
}

void deinitScreen(Screen screen) {
	SDL_DestroyRenderer(screen.renderer);
	SDL_DestroyWindow(screen.window);
}

void renderFrame(Frame frame, Screen screen) {
	Uint32 rmask, gmask, bmask, amask;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	amask = 0x00000000;

	int depth, pitch;
	depth = 32;
	pitch = 4 * WIDTH;

	SDL_Surface * image = SDL_CreateRGBSurfaceFrom((void*)frame.content, WIDTH, HEIGHT,
		depth, pitch, rmask, gmask, bmask, amask);

	SDL_Texture * texture = SDL_CreateTextureFromSurface(screen.renderer, image);
	SDL_RenderCopy(screen.renderer, texture, NULL, NULL);
	SDL_RenderPresent(screen.renderer);
	SDL_UpdateWindowSurface(screen.window);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
}