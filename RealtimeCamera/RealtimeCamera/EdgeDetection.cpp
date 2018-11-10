#include "stdafx.h"
#include "ImageProcessing.h"
#include <algorithm>
#include <thread>

#define EDGE ((0x00 << 24) | ((uint8_t)255 << 16) | ((uint8_t)255 << 8) | (uint8_t)255)
#define NO_EDGE 0

void doGradientStrengthRange(int* source, int egdeDir[], float gradient[], int start, int end, int GxMask[3][3], int GyMask[3][3]);
void suppressNonMax(int* outFrame, float* gradient, int* edgeDir, int pX, int pY, int xDir, int yDir);

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

	std::thread threadArray[NumerOfThreads];
	for (int i = 0; i < NumerOfThreads; i++) {
		threadArray[i] = std::thread(doGradientStrengthRange, frame.content, egdeDir, gradient, i*HEIGHT / NumerOfThreads, (i + 1)*HEIGHT/ NumerOfThreads, GxMask, GyMask);
	}
	for (int i = 0; i < NumerOfThreads; i++) {
		threadArray[i].join();
	}


}

void doGradientStrengthRange(int* source, int egdeDir[], float gradient[], int start, int end, int GxMask[3][3], int GyMask[3][3]) {
	int cord = -1;
	int Gx, Gy;
	float directAngle;
	int angle;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			Gx = 0;
			Gy = 0;

			for (int b = 0; b < 3; b++) {
				for (int a = 0; a < 3; a++) {
					cord = 0; // pixel in frame based on a and b offsets
					if (y + b - 1 < 0) cord += 0;
					else if (y + b - 1 >= HEIGHT) cord += (HEIGHT - 1) * WIDTH;
					else cord += (y + b - 1) * WIDTH;

					if (x + a - 1 < 0) cord += 0;
					else if (x + a - 1 >= WIDTH) cord += WIDTH - 1;
					else cord += x + a - 1;

					Gx += getRed(source[cord]) * GxMask[a][b];
					Gy += getRed(source[cord]) * GyMask[a][b];
				}
			}

			gradient[y*WIDTH + x] = sqrt(Gx * Gx + Gy * Gy);  // gradient strength
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
			egdeDir[y*WIDTH + x] = angle;
		}
	}
}

void followEdge(int width, int height, int* outFrame, float* gradient, int* edgeDir, int pX, int pY, int xDir, int yDir, int edgeStartLimit, int edgeContinueLimit) {
	int newX = pX + xDir;
	int newY = pY + yDir;
	outFrame[pY*width + pX] = EDGE;
	while (newX >= 0 and newX < width and newY >= 0 and newY < height
		and gradient[newY*width + newX] > edgeContinueLimit and edgeDir[newY*width + newX] == edgeDir[pY*width + pX]
		and outFrame[newY*width + newX] == 0) { // decent gragient, no-edge in frame
		outFrame[newY*width + newX] = EDGE;
		newX = pX + xDir;
		newY = pY + yDir;
	}
	newX = pX - xDir;
	newY = pY - yDir;
	while (newX >= 0 and newX < width and newY >= 0 and newY < height
		and gradient[newY*width + newX] > edgeContinueLimit and edgeDir[newY*width + newX] == edgeDir[pY*width + pX] 
		and outFrame[newY*width + newX] == 0) { // decent gragient, no-edge in frame
		outFrame[newY*width + newX] = EDGE;
		newX = pX - xDir;
		newY = pY - yDir;
	}
}

void suppressGradient(float* gradientOut, float* gradient, int* edgeDir) {
	int cord;
	for (int y = 1; y < HEIGHT - 1; y++) {
		for (int x = 1; x < WIDTH - 1; x++) {
			cord = y * WIDTH + x;
			const int nn = cord - WIDTH;
			const int ss = cord + WIDTH;
			const int ww = cord - 1;
			const int ee = cord + 1;
			const int nw = nn + 1;
			const int ne = nn - 1;
			const int sw = ss + 1;
			const int se = ss - 1;

			gradientOut[cord] = gradient[cord];
			switch (edgeDir[cord]) {
			case 90:
				if (gradient[cord] < gradient[nn] or gradient[cord] < gradient[ss]) gradientOut[cord] = 0;
				break;
			case 45:
				if (gradient[cord] < gradient[nw] or gradient[cord] < gradient[se]) gradientOut[cord] = 0;
				break;
			case 0:
				if (gradient[cord] < gradient[ww] or gradient[cord] < gradient[ee]) gradientOut[cord] = 0;
				break;
			case 135:
				if (gradient[cord] < gradient[sw] or gradient[cord] < gradient[ne]) gradientOut[cord] = 0;
				break;
			}
		}
	}
}

void findAllEndes(Frame sourceFrame, Frame* outFrame, int edgeStartLimit, int edgeContinueLimit) {


	int W = WIDTH;
	int H = HEIGHT;
	int cord;

	int* edgeDir = new int[WIDTH * HEIGHT];
	float* gradient = new float[WIDTH * HEIGHT];
	float* gradient1 = new float[WIDTH * HEIGHT];
	int edges[WIDTH *2][2];

	findGradientAndStrength(sourceFrame, edgeDir, gradient1);

	// Non-Max suppression
	suppressGradient(gradient, gradient1, edgeDir);

	memset(outFrame->content, NO_EDGE, WIDTH * HEIGHT * sizeof(int));

	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {

			cord = y * WIDTH + x;

			if (gradient[cord] > edgeContinueLimit && outFrame->content[cord] == NO_EDGE) { // trace edges
				outFrame->content[cord] = EDGE;
				int nedges = 1;
				edges[0][0] = x;
				edges[0][1] = y;

				while (nedges > 0) {
					nedges--;
					const int px = edges[nedges][0];
					const int py = edges[nedges][1];

					int nbs[8]; // neighbours
					if(py >= 1)	
						nbs[0] = (py - 1) * WIDTH + px;	// nn
					else
						nbs[0] = py * WIDTH + px;		// nn
					if (py < H-1)
						nbs[1] = (py + 1) * WIDTH + px;	// ss
					else
						nbs[1] = py * WIDTH + px;		// ss
					if (px < W-1) {
						nbs[2] = px + 1;		// ww
						nbs[4] = nbs[0] + 1;	// nw
						nbs[6] = nbs[1] + 1;	// sw
					}
					else {
						nbs[2] = px;			// ww
						nbs[4] = nbs[0];		// nw
						nbs[6] = nbs[1];		// sw
					}
					if (px >= 1) {
						nbs[3] = px - 1;     // ee
						nbs[5] = nbs[0] - 1; // ne
						nbs[7] = nbs[1] - 1; // se
					}
					else {
						nbs[3] = px;     // ee
						nbs[5] = nbs[0]; // ne
						nbs[7] = nbs[1]; // se
					}

					for (int k = 0; k < 8; k++)
						if (gradient[nbs[k]] >= edgeContinueLimit && outFrame->content[nbs[k]] == NO_EDGE) {
							outFrame->content[nbs[k]] = EDGE;
							edges[nedges][0] = nbs[k] % WIDTH;
							edges[nedges][1] = nbs[k] / WIDTH;
							nedges++;
						}
				}
			}
		}
	}
	delete(edgeDir);
	delete(gradient);
	delete(gradient1);
}

void suppressNonMax(int* outFrame, float* gradient, int* edgeDir, int pX, int pY, int xDir, int yDir) {
	int dir = edgeDir[pY*WIDTH + pX];
	int newX = 0;
	int newY = 0;
	int cord = 0;
	bool edgeEnd = false;
	float* nonMax = new float[HEIGHT * WIDTH * 3];		// Temporarily stores gradients and positions of pixels in parallel edges
	int pixelCount = 0;		// Stores the number of pixels in parallel edges
	int nonMaxCord = 0;
	int max[3];			// Maximum point in a wide edge

	newX = pX + xDir;
	newY = pY + yDir;
	if (newX < 0 or newX >= WIDTH or newY < 0 or newY >= HEIGHT) edgeEnd = true;
	cord = newY * WIDTH + newX;

	// Find non-maximum parallel edges tracing up 
	while (!edgeEnd and edgeDir[cord] == dir and outFrame[cord] == EDGE) { // czy linia obok idzie w t¹ sam¹ strone + jest lini¹ + sie nie skoñczy³a
		
		// parallel line exists
		nonMaxCord = pixelCount * 3; 
		nonMax[nonMaxCord] = newX;
		nonMax[nonMaxCord+1] = newY;
		nonMax[nonMaxCord+2] = gradient[cord];
		pixelCount++;
		
		newX += xDir;
		newY += yDir;
		if (newX < 0 or newX >= WIDTH or newY < 0 or newY >= HEIGHT) edgeEnd = true;
		cord = newY * WIDTH + newX;
	}

	// Find non-maximum parallel edges tracing down 
	edgeEnd = false;
	xDir *= -1;
	yDir *= -1;

	newX = pX + xDir;
	newY = pY + yDir;
	if (newX < 0 or newX >= WIDTH or newY < 0 or newY >= HEIGHT) edgeEnd = true;
	cord = newY * WIDTH + newX;

	while (!edgeEnd and edgeDir[cord] == dir and outFrame[cord] == EDGE) { // czy linia obok idzie w t¹ sam¹ strone + jest lini¹ + sie nie skoñczy³a

		// parallel line exists
		nonMaxCord = pixelCount * 3;
		//std::cout << " " << pixelCount << " ";
		nonMax[nonMaxCord] = newX;
		nonMax[nonMaxCord + 1] = newY;
		nonMax[nonMaxCord + 2] = gradient[cord];
		pixelCount++;

		newX += xDir;
		newY += yDir;
		if (newX < 0 or newX >= WIDTH or newY < 0 or newY >= HEIGHT) edgeEnd = true;
		cord = newY * WIDTH + newX;
	}

	// Suppress non-maximum edges 
	max[0] = 0;
	max[1] = 0;
	max[2] = 0;
	for (int count = 0; count < pixelCount; count++) {
		nonMaxCord = count * 3;
		if (nonMax[nonMaxCord+2] > max[2]) { // znalezienie najsilnieszego gradientu w linii
			max[0] = nonMax[nonMaxCord];
			max[1] = nonMax[nonMaxCord + 1];
			max[2] = nonMax[nonMaxCord + 2];
		}
	}
	for (int count = 0; count < pixelCount; count++) {
		nonMaxCord = count * 3;
		cord = nonMax[nonMaxCord + 1] * WIDTH + nonMax[nonMaxCord];
		outFrame[cord] = NO_EDGE;
	}

	free(nonMax);
}
