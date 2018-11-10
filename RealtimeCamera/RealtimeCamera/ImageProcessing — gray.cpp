#include "ImageProcessing.h"
#include "stdafx.h"
#include <algorithm>
#include <thread>

uint8_t getRed(int px) {
	return (px >> 16) & 0xff;
}
uint8_t getGreen(int px) {
	return (px >> 8) & 0xff;
}
uint8_t getBlue(int px) {
	return (px >> 0) & 0xff;
}
uint8_t getAlpha(int px) {
	return (px >> 24) & 0xff;
}
void initFrame(Frame* frame, int width, int height) {
	frame->width = width;
	frame->height = height;
	frame->content = new int[width * height];
}
void deinitFrame(Frame* frame) {
	delete(frame->content);
}
int convertPixelRGBtoGrayscale(int RGBPixel) {
	uint8_t a = (RGBPixel >> 24) & 0xff;
	uint8_t r = (RGBPixel >> 16) & 0xff;
	uint8_t g = (RGBPixel >> 8) & 0xff;
	uint8_t b = (RGBPixel >> 0) & 0xff;

	uint8_t v = (r + g + b) / 3;
	return (a << 24) | (v << 16) | (v << 8) | v;
}

void createGrayFrameFromMap(Frame* frame, float* tab, int width, int height, float light) {
	if (frame->height != height or frame->width != width) {
		deinitFrame(frame);
		initFrame(frame, width, height);
	}

	for (int i = 0; i < width * height; i++) {
		frame->content[i] = (0x00<< 24) | ((uint8_t)(tab[i]*light) << 16) | ((uint8_t)(tab[i]* light) << 8) | (uint8_t)(tab[i]* light);
	}

}

// GAUSSIAN BLUR WITH GRAYSCALE
void performeGaussianBlurInRange(Frame source, int* image, int start, int end, int gausMatrix[5][5], int gausDiv, float light) {
	int px;
	int W = source.width;
	int H = source.height;
	int cord = -1;

	// GAUSSIAN BLUR
	for (int y = start; y <= end and y<H; y++) {
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

					//cord = (y + b - 2) * W + x + a - 2;

					px += getRed(source.content[cord]) * gausMatrix[a][b] / gausDiv / 3;
					px += getBlue(source.content[cord]) * gausMatrix[a][b] / gausDiv / 3;
					px += getGreen(source.content[cord]) * gausMatrix[a][b] / gausDiv / 3;

				}
			}
			/*
			px += getRed(source.content[x + y * W]) / 3;
			px += getBlue(source.content[x + y * W]) / 3;
			px += getGreen(source.content[x + y * W])  / 3;		*/
			//std::cout << x + y * W << std::endl;
			px *= light;
			px = std::min(px, 255);
			image[x + y * W] = (0x00 << 24) | ((uint8_t)px << 16) | ((uint8_t)px << 8) | (uint8_t)px;
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
	int W = sourceFrame.width;
	int H = sourceFrame.height;

	//for (int i = 0; i < W*H; i++) outFrame->content[i] = 0;

	std::thread threadArray[threadNumber];

	for (int i = 0; i < threadNumber; i++) {
		threadArray[i] = std::thread(performeGaussianBlurInRange, sourceFrame, outFrame->content, i*H / threadNumber, (i + 1)*H / threadNumber, gausMatrix, gausDiv, light);
	}
	for (int i = 0; i < threadNumber; i++) {
		threadArray[i].join();
	}
}

// ENDGE DETECTION + GRADIENT
void findGradientAndStrength(Frame frame, int egdeDir[], float gradient[]) {
	int GxMask[3][3] = { // Sobel mask in x direction
		{ -1, 0, 1 },
		{ -2, 0, 2 },
		{ -1, 0, 1 },
	};
	int GyMask[3][3] = { // Sobel mask in y direction
		{ 1,  2,  1 },
		{ 0,  0,  0 },
		{ -1, -2, -1 },
	};
	int W = frame.width;
	int H = frame.height;
	int cord = -1;
	int Gx, Gy;
	float directAngle;
	int angle;

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			Gx = 0;
			Gy = 0;

			for (int b = 0; b < 3; b++) {
				for (int a = 0; a < 3; a++) {
					cord = 0; // pixel in frame based on a and b offsets
					if (y + b - 1 < 0) cord += 0;
					else if (y + b - 1 >= H) cord += (H - 1) * W;
					else cord += (y + b - 1) * W;

					if (x + a - 1 < 0) cord += 0;
					else if (x + a - 1 >= W) cord += W - 1;
					else cord += x + a - 1;

					Gx += getRed(frame.content[cord]) * GxMask[a][b];
					Gy += getRed(frame.content[cord]) * GyMask[a][b];
				}
			}

			gradient[y*W + x] = sqrt(Gx * Gx + Gy * Gy);  // gradient strength
			directAngle = (atan2(Gx, Gy) / 3.14159) * 180.0; // edge direction (-180 : +180)

			// conversion to directional angle from right up
			if (directAngle < -157.5 or directAngle >= 157.5 or (directAngle >= -22.5 and directAngle < 22.5)) angle = 0;
			else if ((directAngle >= 22.5 and directAngle < 67.5) or (directAngle >= -157.5 and directAngle < -112.5)) angle = 45;
			else if ((directAngle >= 67.5 and directAngle < 112.5) or (directAngle >= -112.5 and directAngle < -67.5)) angle = 90;
			else if ((directAngle >= 112.5 and directAngle < 157.5) or (directAngle >= -67.5 and directAngle < -22.5)) angle = 135;
			else {
				std::cout << "ERROR findGradientAndStrength: Incorrect angle" << std::endl;
				exit(-1);
			}
			egdeDir[y*W + x] = angle;
		}
	}

}
void findAllEndes(Frame sourceFrame, Frame* outFrame, int edgeLimit) {
	int W = sourceFrame.width;
	int H = sourceFrame.height;
	if (outFrame->height != H or outFrame->width != W) {
		deinitFrame(outFrame);
		initFrame(outFrame, W, H);
	}

	int* egdeDir = new int[W * H];
	float* gradient = new float[W * H];
	const int EDGE = (0x00 << 24) | ((uint8_t)255 << 16) | ((uint8_t)255 << 8) | (uint8_t)255;
	const int NO_EDGE = 0; // (0x00 << 24) | ((uint8_t)255 << 16) | ((uint8_t)255 << 8) | (uint8_t)255;

	findGradientAndStrength(sourceFrame, egdeDir, gradient);

	for (int y = 1; y < H - 1; y++) {
		for (int x = 1; x < W - 1; x++) {
			if (gradient[y*W + x] < edgeLimit) {
				outFrame->content[y*W + x] = NO_EDGE;
				continue;
			}
			switch (egdeDir[y*W + x])
			{
			case 0:
				if (gradient[y*W + x + 1] > edgeLimit or gradient[y*W + x - 1] > edgeLimit)
					outFrame->content[y*W + x] = EDGE;
				else
					outFrame->content[y*W + x] = NO_EDGE;
				break;
			case 45:
				if (gradient[(y-1) * W + x - 1] > edgeLimit or gradient[(y+1) * W + x + 1] > edgeLimit)
					outFrame->content[y*W + x] = EDGE;
				else
					outFrame->content[y*W + x] = NO_EDGE;
				break;
			case 90:
				if (gradient[(y + 1) * W + x] > edgeLimit or gradient[(y - 1) * W + x] > edgeLimit)
					outFrame->content[y*W + x] = EDGE;
				else
					outFrame->content[y*W + x] = NO_EDGE;
				break;
			case 135:
				if (gradient[(y + 1) * W + x - 1] > edgeLimit or gradient[(y - 1) * W + x + 1] > edgeLimit)
					outFrame->content[y*W + x] = EDGE;
				else
					outFrame->content[y*W + x] = NO_EDGE;
				break;
			default:
				std::cout << "ERROR findAllEndes: no angle" << std::endl;
			}
		}
	}

}

