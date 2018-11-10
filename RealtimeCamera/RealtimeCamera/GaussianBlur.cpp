#include "stdafx.h"
#include "ImageProcessing.h"
#include <algorithm>
#include <thread>

void doRangeGaussian(int* source, int start, int end, int gausMatrix[5][5], int gausDiv);
void doRefillValues(int* source, int start, int end);

void performGaussianBlurOnGrayscale(Frame* frame) {
	int gausDiv = 159;
	int gausMatrix[5][5] = {
		{ 2,  4,  5,  4, 2 },
		{ 4,  9, 12,  9, 4 },
		{ 5, 12, 15, 12, 5 },
		{ 4,  9, 12,  9, 4 },
		{ 2,  4,  5,  4, 2 },
	};

	int W = WIDTH;
	int H = HEIGHT;

	std::thread threadArray[NumerOfThreads];
	
	for (int i = 0; i < NumerOfThreads; i++) {
		threadArray[i] = std::thread(doRangeGaussian, frame->content, i*H / NumerOfThreads, (i + 1)*H / NumerOfThreads, gausMatrix, gausDiv);
	}
	for (int i = 0; i < NumerOfThreads; i++) {
		threadArray[i].join();
	}

	for (int i = 0; i < NumerOfThreads; i++) {
		threadArray[i] = std::thread(doRefillValues, frame->content, i*H / NumerOfThreads, (i + 1)*H / NumerOfThreads);
	}
	for (int i = 0; i < NumerOfThreads; i++) {
		threadArray[i].join();
	}

	//*/
	/*
	int cord;
	int px;

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			px = 0;
			for (int b = 0; b < 5; b++) {
				for (int a = 0; a < 5; a++) {
					cord = 0; // pixel in frame based on a and b offsets
					if (y + b - 2 < 0) cord += 0;
					else if (y + b - 2 >= H) cord += (H - 1) * W;
					else cord += (y + b - 2) * W;

					if (x + a - 2 < 0) cord += 0;
					else if (x + a - 2 >= W) cord += W - 1;
					else cord += x + a - 2;

					px += getBlue(frame->content[cord]) * gausMatrix[a][b];
				}
			}
			px /= gausDiv;
			setRed(&frame->content[y*W + x], (uint8_t) px);
		}
	for (int y = 0; y < H; y++)
		for (int x = 0; x < W; x++)
			setAllColors(&frame->content[y*W + x], getRed(frame->content[y*W + x]), getRed(frame->content[y*W + x]), getRed(frame->content[y*W + x]));
	} //*/
		
}

void doRangeGaussian(int* source, int start, int end, int gausMatrix[5][5], int gausDiv) {
	int cord;
	int px;
	for (int y = start; y < end; y++) {
		for (int x = 0; x < WIDTH; x++) {
			px = 0;
			for (int b = 0; b < 5; b++) {
				for (int a = 0; a < 5; a++) {
					cord = 0; // pixel in frame based on a and b offsets
					if (y + b - 2 < 0) cord += 0;
					else if (y + b - 2 >= HEIGHT) cord += (HEIGHT - 1) * WIDTH;
					else cord += (y + b - 2) * WIDTH;

					if (x + a - 2 < 0) cord += 0;
					else if (x + a - 2 >= WIDTH) cord += WIDTH - 1;
					else cord += x + a - 2;

					px += getBlue(source[cord]) * gausMatrix[a][b];
				}
			}
			px /= gausDiv;
			setRed(&source[y*WIDTH + x], (uint8_t)px);
		}
	}
}

void doRefillValues(int* source, int start, int end) {
	for (int y = start; y < end; y++)
		for (int x = 0; x < WIDTH; x++)
			setAllColors(&source[y*WIDTH + x], getRed(source[y*WIDTH + x]), getRed(source[y*WIDTH + x]), getRed(source[y*WIDTH + x]));
}

// GAUSSIAN BLUR WITH GRAYSCALE
void performeGaussianBlurInRange(Frame source, int* image, int start, int end, int gausMatrix[5][5], int gausDiv, float light) {
	int px;
	int pxRed, pxGreen, pxBlue;
	int cord = -1;
	int gray;
	int conversionFactor, averageValue;
	int numberOfShades = 32;

	// GAUSSIAN BLUR
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = start; x < WIDTH and x < end; x++) {
			px = 0;
			pxRed = pxGreen = pxBlue = 0;
			for (int b = 0; b < 5; b++) {
				for (int a = 0; a < 5; a++) {
					cord = 0; // pixel in frame based on a and b offsets
					if (y + b - 2 < 0) cord += 0;
					else if (y + b - 2 >= HEIGHT) cord += (HEIGHT - 1) * WIDTH;
					else cord += (y + b - 2) * WIDTH;

					if (x + a - 2 < 0) cord += 0;
					else if (x + a - 2 >= WIDTH) cord += WIDTH - 1;
					else cord += x + a - 2;

					//cord = (y + b - 2) * W + x + a - 2;
					conversionFactor = 255 / (numberOfShades - 1);
					averageValue = (getRed(source.content[cord]) + getBlue(source.content[cord]) + getGreen(source.content[cord])) / 3;
					gray = ((averageValue / conversionFactor) + 0.5) * conversionFactor;
					px += gray * gausMatrix[a][b] / gausDiv;


					//px += getRed(source.content[cord]) * gausMatrix[a][b] / gausDiv * 0.299;
					//px += getBlue(source.content[cord]) * gausMatrix[a][b] / gausDiv * 0.587;
					//px += getGreen(source.content[cord]) * gausMatrix[a][b] / gausDiv * 0.114;
					/*
					pxRed += getRed(source.content[cord]) * gausMatrix[a][b] / gausDiv;
					pxGreen += getBlue(source.content[cord]) * gausMatrix[a][b] / gausDiv;
					pxBlue += getGreen(source.content[cord]) * gausMatrix[a][b] / gausDiv; */

				}
			}
			/*
			px += getRed(source.content[x + y * W]) / 3;
			px += getBlue(source.content[x + y * W]) / 3;
			px += getGreen(source.content[x + y * W])  / 3;		*/
			//std::cout << x + y * W << std::endl;
			px *= light;
			px = std::min(px, 255);
			image[x + y * WIDTH] = (0x00 << 24) | ((uint8_t)px << 16) | ((uint8_t)px << 8) | (uint8_t)px;
		}
	}
}

void gaussianBlurGrayscaleFrame(Frame sourceFrame, Frame *outFrame, float light) {
	const int threadNumber = 4;
	int gausDiv = 159;
	int gausMatrix[5][5] = {
		{ 2,  4,  5,  4, 2 },
	{ 4,  9, 12,  9, 4 },
	{ 5, 12, 15, 12, 5 },
	{ 4,  9, 12,  9, 4 },
	{ 2,  4,  5,  4, 2 },
	};
	int W = WIDTH;
	int H = HEIGHT;

	//for (int i = 0; i < W*H; i++) outFrame->content[i] = 0;

	std::thread threadArray[threadNumber];

	for (int i = 0; i < threadNumber; i++) {
		threadArray[i] = std::thread(performeGaussianBlurInRange, sourceFrame, outFrame->content, i*W / threadNumber, (i + 1)*W / threadNumber, gausMatrix, gausDiv, light);
	}
	for (int i = 0; i < threadNumber; i++) {
		threadArray[i].join();
	}
}