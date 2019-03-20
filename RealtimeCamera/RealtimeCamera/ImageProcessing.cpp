#include "stdafx.h"
#include "ImageProcessing.h"
#include <algorithm>
#include <thread>

// SET & GET
uint8_t getRed(int px)	 {	return (px >> 16) & 0xff;	}
uint8_t getGreen(int px) {	return (px >> 8) & 0xff;	}
uint8_t getBlue(int px)  {	return (px >> 0) & 0xff;	}
uint8_t getAlpha(int px) {	return (px >> 24) & 0xff;	}
void setAllColors(int* px, uint8_t red, uint8_t green, uint8_t blue) { *px = (*px << 24) | (red<< 16) | (green << 8) | blue;	}
void setRed(int* px, uint8_t red) {
	int green = (*px >> 16) & 0xff, blue = (*px >> 0) & 0xff, alpha = (*px >> 24) & 0xff;
	*px = (alpha << 24) | (red << 16) | (green << 8) | blue;
}
void setGreen(int* px, uint8_t green) {
	int red = (*px >> 16) & 0xff, blue = (*px >> 0) & 0xff, alpha = (*px >> 24) & 0xff;
	*px = (alpha << 24) | (red << 16) | (green << 8) | blue;
}
void setBlue(int* px, uint8_t blue) {
	int red = (*px >> 16) & 0xff, green = (*px >> 16) & 0xff, alpha = (*px >> 24) & 0xff;
	*px = (alpha << 24) | (red << 16) | (green << 8) | blue;
}
void setAlpha(int* px, uint8_t alpha) {
	int red = (*px >> 16) & 0xff, green = (*px >> 16) & 0xff, blue = (*px >> 0) & 0xff;
	*px = (alpha << 24) | (red << 16) | (green << 8) | blue;
}

// BASIC FRAME OPERATIONS
void initFrame(Frame* frame) {
	frame->content = new int[WIDTH * HEIGHT];
}

void deinitFrame(Frame* frame) {
	delete(frame->content);
}

void copyFrame(Frame source, Frame * dest) {

	memcpy(dest->content, source.content, WIDTH * HEIGHT * sizeof(int));

	//for (int i = 0; i < source.width * source.height; i++)
		//dest->content[i] = source.content[i];

}

void clearFrame(Frame* frame) {
	memset(frame->content, 0, WIDTH * HEIGHT * sizeof(int));
}

void averageFrames(Frame** frameTab, int n) {
	//if (n == 1) return;
	int avgRed, avgGreen, avgBlue;
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		//average2FramesWeight(frameTab[0], frameTab[i], i, 1);
		avgRed = avgGreen = avgBlue = 0;
		for (int a = 0; a < n; a++) {
			avgRed += getRed(frameTab[0]->content[i]);
			avgGreen += getGreen(frameTab[0]->content[i]);
			avgBlue += getBlue(frameTab[0]->content[i]);
		}
		avgRed /= n;
		avgGreen /= n;
		avgBlue /= n;
		setAllColors(&frameTab[0]->content[i], avgRed, avgGreen, avgBlue);
		//frameTab[0]->content[i]=(0x00 << 24) | ((uint8_t)(avgRed) << 16) | ((uint8_t)(avgGreen) << 8) | (uint8_t)(avgBlue);
	}
}

void average2FramesWeight(Frame* f1, Frame* f2, int w1, int w2) {
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		setRed(	&(f1->content[i]),	(getRed(f1->content[i]) * w1 + getRed(f2->content[i]) * w2) / (w1+w2));
		setGreen(&(f1->content[i]), (getGreen(f1->content[i]) * w1 + getGreen(f2->content[i]) * w2) / (w1 + w2));
		setBlue(&(f1->content[i]), (getBlue(f1->content[i]) * w1 + getBlue(f2->content[i]) * w2) / (w1 + w2));
	}
}

int convertPixelRGBtoGrayscale(int RGBPixel) {
	uint8_t a = (RGBPixel >> 24) & 0xff;
	uint8_t r = (RGBPixel >> 16) & 0xff;
	uint8_t g = (RGBPixel >> 8) & 0xff;
	uint8_t b = (RGBPixel >> 0) & 0xff;

	uint8_t v = (r + g + b) / 3;
	return (a << 24) | (v << 16) | (v << 8) | v;
}

void preformGrayscale(Frame* frame) {

	auto grayPart = [](Frame* frame, int start, int end) -> void {
		for (int y = start; y < end; y++)
			for (int x = 0; x < WIDTH; x++)
				frame->content[y*WIDTH + x] = convertPixelRGBtoGrayscale(frame->content[y*WIDTH + x]);
	};

	std::thread threadArray[NumerOfThreads];
	for (int i = 0; i < NumerOfThreads; i++) {
		threadArray[i] = std::thread(grayPart, frame, i*HEIGHT / NumerOfThreads, (i + 1)*HEIGHT / NumerOfThreads);
	}
	for (int i = 0; i < NumerOfThreads; i++) {
		threadArray[i].join();
	}

}

void createGrayFrameFromMap(Frame* frame, float* tab, float light) {
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		frame->content[i] = (0x00<< 24) | ((uint8_t)(tab[i]*light) << 16) | ((uint8_t)(tab[i]* light) << 8) | (uint8_t)(tab[i]* light);
	}

}


int maxColorDifference(int px1, int px2) {
	return std::max(abs(getRed(px1) - getRed(px2)), std::max(abs(getGreen(px1) - getGreen(px2)), abs(getBlue(px1) - getBlue(px2))));
}



/*
void doComplexCapture(Frame* mainFrame, Frame* helpFrame, int captures = 1) {
	captureFrame(mainFrame);
	if (captures == 1) {
	return;
	}
	copyFrame(*mainFrame, helpFrame);
	captureFrame(mainFrame);
	average2FramesWeight(mainFrame, helpFrame, 1, 1);
	for (int i = 2; i < captures; i++) {
	captureFrame(helpFrame);
	average2FramesWeight(mainFrame, helpFrame, i - 1, 1);
}
*/