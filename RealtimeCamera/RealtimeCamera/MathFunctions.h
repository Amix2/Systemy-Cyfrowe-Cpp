#pragma once

#include "stdafx.h"
#include "Line.h"
#include "Angle.h"
#include <math.h>
#define NOT_VALID_VCTOR 88.8

struct Vector {
	double angle;
};


Vector initVector(Point p1, Point p2);
Vector initVector();
Vector averageVectorDirections(Vector v1, Vector v2, int w1=1, int w2=1);
bool isBetween(Vector borderMin, Vector borderMax, Vector vec);

void print(Vector v);
void print(Point p);

bool isInFrame(Point p);
double distance(Point p1, Point p2);
Point avgPoint(const Point& p1, const Point& p2);
Point avgPoint(const Point& p1, const Point& p2, const int w1, const int w2);
double differenceVector(Vector v1, Vector v2);
bool isVectorClose(Vector base, Vector comp, double delta);
int getNextDir(int dir, int step);
int compareDir(int base, int comp);

int getDirectionFromMove(int move);
int getMoveFomDirection(int dir);
int getMoveFromPoints(Point start, Point end);
Point performMoveFromDirection(Point point, int dir);

Point neighbours(Point p, int num);

Point findCrossPointLine(const Line& line1, const Line& line2);
Line findCrossLine(const Line& l1, const Line& l2);
