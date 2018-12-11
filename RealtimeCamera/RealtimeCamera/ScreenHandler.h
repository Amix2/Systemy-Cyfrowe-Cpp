#pragma once
#include "stdafx.h"
#include <SDL.h>
#include "escapi.h"
typedef struct Screen {
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;
} Screen;

void initScreen(int, int, Screen*);
void deinitScreen(Screen);
void renderFrame(Frame, Screen);
