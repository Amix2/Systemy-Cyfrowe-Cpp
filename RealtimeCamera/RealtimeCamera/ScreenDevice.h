#pragma once

#include <SDL.h>
#include "escapi.h"
#include "Frame.h"

class ScreenDeice {
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;
	SDL_Surface * image = NULL;
	SDL_Texture * texture = NULL;
public:
	ScreenDeice(int, int);
	~ScreenDeice();
	void render(FrameRGB*);
	void render(Frame1B*);
};
