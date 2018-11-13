#pragma once

#include "stdafx.h"
#include <math.h>
#define NOT_VALID_VCTOR 88.8

struct VectorAngle {
	double angle;
};



VectorAngle initVector(Point p1, Point p2);
VectorAngle initVector();
VectorAngle averageVectorDirections(VectorAngle v1, VectorAngle v2, int w1=1, int w2=1);
bool isBetween(VectorAngle borderMin, VectorAngle borderMax, VectorAngle vec);

void print(VectorAngle v);
void print(Point p);

bool isInFrame(Point p);
double distance(Point p1, Point p2);
double differenceVector(VectorAngle v1, VectorAngle v2);
bool isVectorClose(VectorAngle base, VectorAngle comp, double delta);
int getNextDir(int dir, int step);
int compareDir(int base, int comp);

int getDirectionFromMove(int move);
int getMoveFomDirection(int dir);
int getMoveFromPoints(Point start, Point end);
Point performMoveFromDirection(Point point, int dir);


Point neighbours(Point p, int num);
