// stdafx.h: dołącz plik do standardowych systemowych plików dołączanych,
// lub specyficzne dla projektu pliki dołączane, które są często wykorzystywane, ale
// są rzadko zmieniane
//

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <cstdint>
#include <iostream>

#define LOG(msg) printf("LOG: %s\n", msg);
#define WIDTH 400
#define HEIGHT 300
#define PI 3.141592653589793238462643383279502884

struct Point {
	int x;
	int y;

	bool operator==(const Point& other)
	{
		return x == other.x and y == other.y;
	}
};

struct Frame {
	int* content = NULL;

	int& operator[](int poz) {
		return content[poz];
	}

	int& operator[](Point p) {
		return content[p.y*WIDTH+p.x];
	}
};


const int NumerOfThreads = 4;


// TODO: W tym miejscu odwołaj się do dodatkowych nagłówków wymaganych przez program
