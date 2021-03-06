/*
	TO DO:
	dodać 1 tablice do funkcji, a nie robić nowych
	addVertexPair - add error handler
	traceLineOneSide - lekkie rozgałęzienie
	LineFinder -> przebaczanie kilku wyjść
	fix adding node to vertex -> adding x -> x
*/

#include "stdafx.h"
#include "ScreenHandler.h"
#include "CameraHandler.h"
#include "ImageProcessing.h"
#include "Vertex.h"
#include "LineFinder.h"
#include "MathFunctions.h"
#include "Stripe.h"
#include "VertexPostprocesing.h"
#include "Parallelogram.h"
#include "Polygon.h"
#include "FigureFinder.h"

#include <windows.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <iomanip>

#include "Tests.h"

using namespace std;

enum ScreenMode {
	NORMAL = 0,
	GRAYSCALE,
	GAUSSIAN,
	GRADIENT,
	GRADIENT_SUPPRESS,
	SHOW_EDGES,
	FIND_LINES,
	FIND_LINES_POSTPROC,
	DETECT_SQUARES
};

//const int screenWidth = 400;
//const int screenHeight = 300;
const int frameCature = 1;
int execGradient = 1;

void doComplexCapture(Frame* frameBase, Frame** frameTab, int captures = 1) {
	if (captures <= 1) captureFrame(frameBase);
	else {
		for (int i = 1; i < captures; i++) {
			captureFrame(frameBase);
			copyFrame(*frameBase, frameTab[i]);
		}
		frameTab[0] = frameBase;
		averageFrames(frameTab, captures);
	}
}

int main(int argc, char ** argv) {

	//doAllTests();

	LOG("Realtime Camera START");
	clock_t time1, time2;
	
	bool quit = false;
	SDL_Event event;
	ScreenMode mode = NORMAL;
	float light = 1;
	int edgeLimit = 160;
	float edgeLimitDiv = 2.8;

	Screen screen;
	initScreen(WIDTH * windowMultiplier, HEIGHT * windowMultiplier, &screen);
	initCamera();
	initVertexTab();
	Frame frameBase;

	Frame frame2;
	initFrame(&frame2);

	Frame* frameTab[frameCature];
	for (int i = 1; i < frameCature; i++) {
		Frame frameToTab;
		frameTab[i] = &frameToTab;
		initFrame(frameTab[i]);
	}

	int* edgeDir = new int[WIDTH * HEIGHT];
	float* gradient = new float[WIDTH * HEIGHT];
	float* gradient2 = new float[WIDTH * HEIGHT];

	//captureFrame(&frameBase);

	while (!quit) {
		cout << mode << endl;
		time2 = clock();

		doComplexCapture(&frameBase, frameTab, frameCature);

		SDL_WaitEventTimeout(&event, 1);
		switch (event.type)
		{
		case SDL_QUIT: 
			quit = true;
			break; 
		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_ESCAPE:
				quit = true;
				break;
			case SDL_SCANCODE_UP:
				edgeLimitDiv += 0.5;
				cout << "edgeLimitDiv: " << edgeLimitDiv << endl;
				break;
			case SDL_SCANCODE_DOWN:
				edgeLimitDiv = max(1.0, edgeLimitDiv - 0.2);
				cout << "edgeLimitDiv: " << edgeLimitDiv << endl;
				break;
			case SDL_SCANCODE_LEFT:
				edgeLimit = max(1, edgeLimit - 5);
				cout << "EDGE DETECTION: " << edgeLimit << endl;
				break;
			case SDL_SCANCODE_RIGHT:
				edgeLimit += 5;
				cout << "EDGE DETECTION: " << edgeLimit << endl;
				break;
			case SDL_SCANCODE_A:
				execGradient = (execGradient + 1) % 2;
				cout << "Gradient: " << execGradient << endl;
				break;
			case SDL_SCANCODE_1:
				mode = NORMAL;
				break;
			case SDL_SCANCODE_2:
				mode = GRAYSCALE;
				break;
			case SDL_SCANCODE_3:
				mode = GAUSSIAN;
				break;
			case SDL_SCANCODE_4:
				mode = GRADIENT;
				break;
			case SDL_SCANCODE_5:
				mode = GRADIENT_SUPPRESS;
				break;
			case SDL_SCANCODE_6:
				mode = SHOW_EDGES;
				break;
			case SDL_SCANCODE_7:
				mode = FIND_LINES;
				break;
			case SDL_SCANCODE_8:
				mode = FIND_LINES_POSTPROC;
				break;
			case SDL_SCANCODE_9:
				mode = DETECT_SQUARES;
				break;
			}
		}
		

		switch (mode)
		{
		case NORMAL:
			renderFrame(frameBase, screen);
			break;
		
		case GRAYSCALE:
			preformGrayscale(&frameBase);
			renderFrame(frameBase, screen);
			break;

		case GAUSSIAN:
			preformGrayscale(&frameBase);
			if(execGradient) performGaussianBlurOnGrayscale(&frameBase);
			renderFrame(frameBase, screen);
			break;
		
		case GRADIENT:
			preformGrayscale(&frameBase);
			if (execGradient) performGaussianBlurOnGrayscale(&frameBase);
			findGradientAndStrength(frameBase, edgeDir, gradient);
			createGrayFrameFromMap(&frame2, gradient, light);
			renderFrame(frame2, screen);
			break;

		case GRADIENT_SUPPRESS:
			preformGrayscale(&frameBase);
			if (execGradient) performGaussianBlurOnGrayscale(&frameBase);
			findGradientAndStrength(frameBase, edgeDir, gradient);
			suppressGradient(gradient2, gradient, edgeDir);
			createGrayFrameFromMap(&frame2, gradient2, light);
			renderFrame(frame2, screen);
			//Sleep(1000);
			break;

		case SHOW_EDGES:
			preformGrayscale(&frameBase);
			if (execGradient) performGaussianBlurOnGrayscale(&frameBase);
			findAllEndes(frameBase, &frame2, edgeLimit, max(1, edgeLimit/edgeLimitDiv));
			renderFrame(frame2, screen);
			break;

		case FIND_LINES:
			preformGrayscale(&frameBase);
			if (execGradient) performGaussianBlurOnGrayscale(&frameBase);
			findAllEndes(frameBase, &frame2, edgeLimit, max(1, edgeLimit / edgeLimitDiv));
			findLanes(frame2);
			printVertexSize();
			//printVertex();
			//generateFrameForVertex(&frame2);
			//generateFancyLineFrameForVertex(&frame2);
			generateFancyLineFrameForVertex(&frame2);
			//putVertexOnFrame(&frame2);
			renderFrame(frame2, screen);
			//Sleep(500);
			break;
		case FIND_LINES_POSTPROC:
			preformGrayscale(&frameBase);
			if (execGradient) performGaussianBlurOnGrayscale(&frameBase);
			findAllEndes(frameBase, &frame2, edgeLimit, max(1, edgeLimit / edgeLimitDiv));
			findLanes(frame2);
			printVertexSize();
			//printVertex();
			//generateFrameForVertex(&frame2);
			vertexPostprocesing();
			generateFancyLineFrameForVertex(&frame2);
			//putVertexOnFrame(&frame2);
			renderFrame(frame2, screen);
			//Sleep(500);
			break;
		case DETECT_SQUARES:
			/*
			addVertexPair({ 64, 9 }, { 62, 53 });
			addVertexPair({ 62, 53 },{ 68, 56 });
			addVertexPair({ 68, 56 }, { 92, 59 });
			addVertexPair({ 92, 59 }, { 96, 15 });
			addVertexPair({ 96, 15 }, { 94, 9 });
			addVertexPair({ 94, 9 }, { 67, 6 });
			addVertexPair({ 67, 6 }, { 64, 9 });
			//addVertexPair({ 10,64 }, { 11,82});


			//addVertexPair({ 56,73 }, { 84,72 });
			//addVertexPair({ 56,73 }, { 67,59 });
			//addVertexPair({ 84,72 }, { 67,59 });

			printVertexSize();
			Sleep(500);
			clearFrame(&frame2);
			findSquaresInVertexIntoFrame(&frame2);
			renderFrame(frame2, screen);
			Sleep(1000);
			generateFancyLineFrameForVertex(&frame2);
			renderFrame(frame2, screen);
			Sleep(1000);
			break;
			*/
			//continue
			preformGrayscale(&frameBase);
			if (execGradient) performGaussianBlurOnGrayscale(&frameBase);
			findAllEndes(frameBase, &frame2, edgeLimit, max(1, edgeLimit / edgeLimitDiv));
			findLanes(frame2);
			vertexPostprocesing();
			printVertexSize();
			clearFrame(&frame2);
			putVertexOnFrame(&frame2, false);
			findSquaresInVertexIntoFrame(&frame2);
			renderFrame(frame2, screen);
			clearVertex();
			//Sleep(1000);
			break;
		}

		time1 = clock();
		cout << "TIME: Loop: " << (time1 - time2) << endl;
		//Sleep(1000);
	}
	delete(edgeDir);
	delete(gradient);
	delete(gradient2);
	printf("Deinit 0\n");
	//for (int i = 1; i < frameCature; i++)
		//deinitFrame(frameTab[i]);
	deinitCamera();
	deinitVertexTab();
	printf("Deinit 1\n");
	deinitFrame(&frameBase);
	printf("Deinit 2\n");
	deinitFrame(&frame2); 
	return 0;
}