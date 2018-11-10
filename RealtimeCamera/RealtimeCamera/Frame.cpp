#pragma once
#include "stdafx.h"
#include "Frame.h"
#include "ImageProcessing.h"
#include <algorithm>
#include <thread>


FrameRGB::FrameRGB(struct SimpleCapParams frameIn) {
	this->frame = frameIn.mTargetBuf;
	this->width = frameIn.mWidth;
	this->height = frameIn.mHeight;
	LOG("Frame RGB Constructior SimpleCapParams")
}
FrameRGB::FrameRGB(int *frameIn, int widthIn, int heightIn) {
	this->frame = frameIn;
	this->width = widthIn;
	this->height = heightIn;
	LOG("Frame 1b Constructor Array")
}
FrameRGB::~FrameRGB() {
	delete(this->frame);
}

int FrameRGB::getHeight() {
	return this->height;
}

int FrameRGB::getWidth() {
	return this->width;
}

uint8_t FrameRGB::getRedFrom(int x, int y) {
	return (this->frame[y*this->width + x] >> 16) & 0xff;
}

uint8_t FrameRGB::getGreenFrom(int x, int y) {
	return (this->frame[y*this->width + x] >> 8) & 0xff;
}

uint8_t FrameRGB::getBlueFrom(int x, int y) {
	return (this->frame[y*this->width + x] >> 0) & 0xff;
}

uint8_t FrameRGB::getRedFrom(int x) {
	return (this->frame[x] >> 16) & 0xff;
}

uint8_t FrameRGB::getGreenFrom(int x) {
	return (this->frame[x] >> 8) & 0xff;
}

uint8_t FrameRGB::getBlueFrom(int x) {
	return (this->frame[x] >> 0) & 0xff;
}

int* FrameRGB::getContent() {
	return (this->frame);
}
Frame1B* FrameRGB::highlightEdges(int maxDifference) {
	LOG("FrameRGB::highlightEdges");
	int div = 273;
	int matrix[5][5] = {
		{1,  4,  7,  4, 1},
		{4, 16, 26, 16, 4},
		{7, 26, 41, 26, 7},
		{4, 16, 26, 16, 4},
		{1,  4,  7,  4, 1 },
	};
	uint16_t *content = new uint16_t[this->getHeight() * this->getWidth()];
	int diff = 0;
	int redC, blueC, greenC, redF, blueF, greenF;

	for (int y = 2; y < this->getHeight() - 3; y++) {
		for (int x = 2; x < this->getWidth() - 3; x++) {
			redC = blueC = greenC = redF = blueF = greenF = 0;
			for (int a = 0; a < 5; a++) {
				for (int b = 0; b < 5; b++) {
					redC += this->getRedFrom(x + a - 2, y + b - 2) * matrix[a][b];
					redF += this->getRedFrom(x + 1 + a - 2, y + 1 + b - 2) * matrix[a][b];
					blueC += this->getBlueFrom(x + a - 2, y + b - 2) * matrix[a][b];
					blueF += this->getBlueFrom(x + 1 + a - 2, y + 1 + b - 2) * matrix[a][b];
					greenC += this->getGreenFrom(x + a - 2, y + b - 2) * matrix[a][b];
					greenC += this->getGreenFrom(x + 1 + a - 2, y + 1 + b - 2) * matrix[a][b];
				}
			}
			diff = std::max(abs(redC - redF), std::max(abs(greenC - greenF), abs(blueC - blueF)));
			content[x + y*this->getWidth()] = (diff > maxDifference*div) ? (uint16_t)0xffff : (uint16_t)0x0000;
		}
	}

	for (int i = 0; i < this->getHeight() * this->getWidth() -1; i++) {
		diff = std::max( abs(this->getBlueFrom(i) - this->getBlueFrom(i + 1)), // MAX ( blue_diff, green_diff, red_diff )
			std::max( abs(this->getGreenFrom(i) - this->getGreenFrom(i + 1)), abs(this->getRedFrom(i) - this->getRedFrom(i + 1))));
		content[i] = (diff > maxDifference) ? (uint16_t)0xffff : (uint16_t)0x0000;
	}
	return new Frame1B(content, this->getWidth(), this->getHeight());
}

void FrameRGB::convertRGBFrametoGrayscale() {
	for (int i = 0; i < this->getWidth() * this->getHeight(); i++) {
		this->getContent()[i] = convertPixelRGBtoGrayscale(this->getContent()[i]);
	}
}

void performeGaussianBlurInRange(FrameRGB* source, int* image, int start, int end, int gausMatrix[5][5], int gausDiv) {
	uint8_t px;
	int W = source->getWidth();
	int H = source->getHeight();

	// GAUSSIAN BLUR
	for (int y = 2; y < H - 2; y++) {
		for (int x = start; x < W - 2 and x < end; x++) {
			px = 0;
			for (int a = 0; a < 5; a++) {
				for (int b = 0; b < 5; b++) {
					px += source->getRedFrom(x + a - 2, y + b - 2) * gausMatrix[a][b] / gausDiv / 3;
					px += source->getGreenFrom(x + a - 2, y + b - 2) * gausMatrix[a][b] / gausDiv / 3;
					px += source->getBlueFrom(x + a - 2, y + b - 2) * gausMatrix[a][b] / gausDiv / 3;
				}
			}
			image[x + y * W] = (0xff << 24) | (px << 16) | (px << 8) | px;
		}
	}
}

FrameRGB* FrameRGB::gaussianBlur() {
	int gausDiv = 159;
	int gausMatrix[5][5] = {
		{ 2,  4,  5,  4, 2 },
		{ 4,  9, 12,  9, 4 },
		{ 5, 12, 15, 12, 5 },
		{ 4,  9, 12,  9, 4 },
		{ 2,  4,  5,  4, 2 },
	};
	int GxMask[3][3] = { // Sobel mask in x direction
		{ -1, 0, 1 },
		{ -2, 0, 2 },
		{ -1, 0, 1 },
	};
	int GyMask[3][3] = { // Sobel mask in y direction
		{  1,  2,  1 },
		{  0,  0,  0 },
		{ -1, -2, -1 },
	};
	int *image = new int[this->getHeight() * this->getWidth()];
	int W = this->getWidth();
	int H = this->getHeight();

	const int threadNumber = 4;
	std::thread threadArray[threadNumber];
	for (int i = 0; i < threadNumber; i++) {
		threadArray[i] = std::thread(performeGaussianBlurInRange, this, image, i*W / threadNumber, (i + 1)*W / threadNumber, gausMatrix, gausDiv);
	}
	for (int i = 0; i < threadNumber; i++) {
		threadArray[i].join();
	}

	return new FrameRGB(image, W, H);
}

/* Frame1B* FrameRGB::highlightEdges(int maxDifference) {
	LOG("FrameRGB::highlightEdges")
		uint16_t *content = new uint16_t[this->getHeight() * this->getWidth()];
	int diff = 0;

	for (int i = 0; i < this->getHeight() * this->getWidth() - 1; i++) {
		diff = std::max(abs(this->getBlueFrom(i) - this->getBlueFrom(i + 1)), // MAX ( blue_diff, green_diff, red_diff )
			std::max(abs(this->getGreenFrom(i) - this->getGreenFrom(i + 1)), abs(this->getRedFrom(i) - this->getRedFrom(i + 1))));
		content[i] = (diff > maxDifference) ? (uint16_t)0xffff : (uint16_t)0x0000;
	}
	return new Frame1B(content, this->getWidth(), this->getHeight());
} */



Frame1B::Frame1B(uint16_t *frameIn, int widthIn, int heightIn) {
	this->frame = frameIn;
	this->width = widthIn;
	this->height = heightIn;
	LOG("Frame 1b Constructor")
}
Frame1B::~Frame1B() {
	delete(this->frame);
}
int Frame1B::getHeight() {
	return this->height;
}
int Frame1B::getWidth() {
	return this->width;
}
uint16_t* Frame1B::getContent() {
	return (this->frame);
}