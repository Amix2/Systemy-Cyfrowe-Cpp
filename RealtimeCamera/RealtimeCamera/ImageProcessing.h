#pragma once

#include "stdafx.h"
#include "Frame.h"

void initFrame(Frame*);
void deinitFrame(Frame*);
void clearFrame(Frame* frame);
void copyFrame(Frame, Frame*);
void averageFrames(Frame**, int);
void average2FramesWeight(Frame*, Frame*, int, int);
void createGrayFrameFromMap(Frame*, float*, float light = 1);
int convertPixelRGBtoGrayscale(int);
void preformGrayscale(Frame*);
void performGaussianBlurOnGrayscale(Frame*);
uint8_t getRed(int);
uint8_t getGreen(int);
uint8_t getBlue(int);
uint8_t getAlpha(int);
void setRed(int*, uint8_t);
void setGreen(int*, uint8_t);
void setBlue(int*, uint8_t);
void setAlpha(int*, uint8_t);
void setAllColors(int* px, uint8_t red = 0xff, uint8_t green = 0xff, uint8_t blue = 0xff);
void gaussianBlurGrayscaleFrame(Frame, Frame*, float light=1);
void findGradientAndStrength(Frame, int[], float[]);
void suppressGradient(float*, float*, int*);
void findAllEndes(Frame, Frame*, int, int);

