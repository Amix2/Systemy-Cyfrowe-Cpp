#pragma once

#include "escapi.h"
#include "stdafx.h"

class Frame1B;

class FrameRGB {
	int *frame;
	int width;
	int height;
	/*	a = ( frame.mTargetBuf[0] >> 24 ) & 0xff;
		r = ( frame.mTargetBuf[0] >> 16 ) & 0xff;
		g = ( frame.mTargetBuf[0] >>  8 ) & 0xff;
		uint8_t b = ( frame.mTargetBuf[0] >>  0 ) & 0xff;	*/
	//void performeGaussianBlurInRange(int*, int, int, int**, int);
public:
	FrameRGB(struct SimpleCapParams);
	FrameRGB(int*, int, int);
	~FrameRGB();
	uint8_t getRedFrom(int, int);
	uint8_t getGreenFrom(int, int);
	uint8_t getBlueFrom(int, int);
	uint8_t getRedFrom(int);
	uint8_t getGreenFrom(int);
	uint8_t getBlueFrom(int);
	int getWidth();
	int getHeight();
	int* getContent();

	Frame1B* highlightEdges(int);
	void convertRGBFrametoGrayscale();
	FrameRGB* gaussianBlur();
};

class Frame1B {
	uint16_t *frame;
	int width;
	int height;
	/*	0 - Black
		1 - White */
public:
	Frame1B(uint16_t*, int, int);
	~Frame1B();
	uint16_t getValueFrom(int, int);
	int getWidth();
	int getHeight();
	uint16_t* getContent();
};