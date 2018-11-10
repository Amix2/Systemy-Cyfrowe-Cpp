#pragma once
#include "stdafx.h"
#include "ScreenDevice.h"

ScreenDeice::ScreenDeice(int width, int height) {
	LOG("ScreenDeice Constructor")
	this->window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
	this->renderer = SDL_CreateRenderer(window, -1, 0);
}
ScreenDeice::~ScreenDeice() {
	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->image);
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}
void ScreenDeice::render(FrameRGB* frame) {
	LOG("ScreenDeice render FrameRGB")
	Uint32 rmask, gmask, bmask, amask;
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	amask = 0x00000000;

	int depth, pitch;
	depth = 32;
	pitch = 4 * frame->getWidth();

	this->image = SDL_CreateRGBSurfaceFrom((void*)frame->getContent(), frame->getWidth(), frame->getHeight(),
		depth, pitch, rmask, gmask, bmask, amask);

	this->texture = SDL_CreateTextureFromSurface(this->renderer, this->image);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->image);
}

void ScreenDeice::render(Frame1B* frame) {
	LOG("ScreenDeice render Frame1B")
	Uint8 rmask, gmask, bmask, amask;
	rmask = 0x0fff;
	gmask = 0x0fff;
	bmask = 0x0fff;
	amask = 0x0000;

	int depth, pitch;
	depth = 16;
	pitch = 2 * frame->getWidth();

	this->image = SDL_CreateRGBSurfaceFrom((void*)frame->getContent(), frame->getWidth(), frame->getHeight(),
		depth, pitch, rmask, gmask, bmask, amask);

	this->texture = SDL_CreateTextureFromSurface(this->renderer, this->image);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(this->texture);
	SDL_FreeSurface(this->image);
}