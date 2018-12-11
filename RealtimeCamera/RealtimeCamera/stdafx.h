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

#define LOG(msg) printf("LOG: %s\n", msg);
#define WIDTH 400
#define HEIGHT 300
const float windowMultiplier = 2.6;
#define PI 3.141592653589793238462643383279502884
const int defaultNodesSize = 100; // Vertex inner node table
const int VertexTabLength = 10000; // global Vertex table
const double VertexProximity = 3; // Vertex proximity search
const double minLineLenght = 2; // lineFinder
#define MAX_DISTANCE 1 // Stripe 0,5 d
#define MERGE_POINT_LINE_DISTANCE 5
#define MERGE_LINE_ANGLE (2/(2*PI))

struct Point {
	int x;
	int y;

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

struct Line {
	double A, B, C;
	Point point[2];

	Line(const Point& point1, const Point& point2) {
		const double con1 = point1.x - point2.x;
		const double con2 = point1.y - point2.y;
		A = -con2;
		B = con1;
		C = point1.x*con2 - point2.y*con1;
		point[0] = point1;
		point[1] = point2;
	}
	Line() {

	}
	Line(int a, int b, int c) {
		A = a;
		B = b;
		C = c;
	}

	double distanceFrom(const Point& point) {
		return abs(A*point.x + B * point.y + C) / sqrt(A*A + B * B);
	}

	bool isAbove(const Point& referencePoint, const Point& point);
	bool isBelow(const Point& referencePoint, const Point& point);
};

struct DPoint {
	double x;
	double y;

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
		if (p.x+p.y == -1) printf("ZEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERA\n");
		return content[((int)p.y)*WIDTH+((int)p.x)];
	}
};


const int NumerOfThreads = 4;

// TODO: W tym miejscu odwołaj się do dodatkowych nagłówków wymaganych przez program
