// stdafx.h: dołącz plik do standardowych systemowych plików dołączanych,
// lub specyficzne dla projektu pliki dołączane, które są często wykorzystywane, ale
// są rzadko zmieniane
//

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <stdlib.h> 
#include <tchar.h>
#include <cstdint>
#include <math.h>
#include <iostream>
#include <algorithm> 
#include <float.h>

#define LOG(msg) printf("LOG: %s\n", msg);
#define WIDTH 600
#define HEIGHT 400
const float windowMultiplier = 1.5;
#define PI 3.141592653589793238462643383279502884
const int defaultNodesSize = 100; // Vertex inner node table
const int VertexTabLength = 10000; // global Vertex table
const double VertexProximity = 5; // Vertex proximity search
const double minLineLenght = 1; // lineFinder
#define MAX_DISTANCE 0.5 // Stripe 0,5 d
#define MERGE_POINT_LINE_DISTANCE 3 // 5
#define MERGE_LINE_ANGLE (1/(2*PI)) // 2
#define MERGE_LINE_LENGTH_PROP 100 // proporcja pomiędzy liniami aby je złączyć
#define POLYGON_SQUARE_APPROX 0.17 // (1-this to 1+this) * SQUARE_SURFACE must contain POLYGON_SURFACE

struct Point {
	int x;
	int y;

	Point(int xx, int yy) {
		x = xx;
		y = yy;
	}
	Point() {}

	bool operator==(const Point& other)
	{
		return x == other.x and y == other.y;
	}

	bool isInLine(const Point& other) {
		return (x == other.x or y == other.y);
	}
	bool isInLineCross(const Point& other) {
		return (x != other.x and y != other.y);
	}
};



struct DPoint {
	double x;
	double y;

	DPoint(double xx, double yy) { x = xx, y = yy; };
	DPoint() {};
	bool operator==(const Point& other)
	{
		return x == other.x and y == other.y;
	}
};

struct Frame {
	int* content = NULL;
	int& operator[](int poz) {
		if (poz == -1) printf("ZEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERA\n");
		return content[poz];
	}

	int& operator[](const Point p) {
		if (p.x+p.y < 0) printf("ZEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERA\n");
		return content[((int)p.y)*WIDTH+((int)p.x)];
	}

	void drawLine(const Point& p1, const Point& p2, uint8_t red = 0xff, uint8_t blue = 0xff, uint8_t green = 0xff) {
		const double con1 = (p1.x) - (p2.x);
		const double con2 = (p1.y) - (p2.y);
		const double A = -con2;
		const double B = con1;
		const double C = p1.x*con2 - p1.y*con1;

		const double a = (double)(p1.y - p2.y) / (p1.x - p2.x);
		const double b = (double)(p1.y + 0.5) - a * (p1.x + 0.5);

		if (std::abs(p1.x - p2.x) > std::abs(p1.y - p2.y)) { // iter XX
			const int Xmax = std::max(p1.x, p2.x);
			const int Xmin = std::min(p1.x, p2.x);
			int itX = Xmin;
			int py = -1;
			while (itX <= Xmax) {
				py = round((A*(itX+0.5) + C) / (-B));
				if (itX >= 0 and itX<WIDTH and py >= 0 and py<HEIGHT) this->content[py * WIDTH + itX]= (this->content[py * WIDTH + itX] << 24) | (red << 16) | (green << 8) | blue;
				itX++;
			}
		}
		else { // iter YY
			const int Ymax = std::max(p1.y, p2.y);
			const int Ymin = std::min(p1.y, p2.y);
			int itY = Ymin;
			int px = -1;
			while (itY <= Ymax) {
				px = round((B*(itY+0.5) + C) / (-A));
				if(itY>=0 and itY<HEIGHT and px>=0 and px<WIDTH) this->content[itY * WIDTH + px] = (this->content[itY * WIDTH + px] << 24) | (red << 16) | (green << 8) | blue;
				itY++;
			}
		}
	}
}; //*px = (*px << 24) | (red<< 16) | (green << 8) | blue;


const int NumerOfThreads = 4;

// TODO: W tym miejscu odwołaj się do dodatkowych nagłówków wymaganych przez program
