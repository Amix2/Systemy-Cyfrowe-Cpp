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
#define WIDTH 400
#define HEIGHT 300
const float windowMultiplier = 2.6;
#define PI 3.141592653589793238462643383279502884
const int defaultNodesSize = 100; // Vertex inner node table
const int VertexTabLength = 10000; // global Vertex table
const double VertexProximity = 3; // Vertex proximity search
const double minLineLenght = 1; // lineFinder
#define MAX_DISTANCE 0.5 // Stripe 0,5 d
#define MERGE_POINT_LINE_DISTANCE 5
#define MERGE_LINE_ANGLE (1/(2*PI)) //2

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
};


const int NumerOfThreads = 4;

// TODO: W tym miejscu odwołaj się do dodatkowych nagłówków wymaganych przez program
