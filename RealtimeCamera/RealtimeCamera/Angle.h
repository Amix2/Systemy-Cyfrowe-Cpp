#pragma once
#include "stdafx.h"
#include "Line.h"
#include <math.h>

#define ZERO_ANGLE 0.00001

double angleDifference(double angle1, double angle2);
int angleCompare(double angle1, double angle2);  // angle1 > angle2
double angleCalculate(Point start, Point end);
double angleCalculate(DPoint start, Point end);
double angleAdd(double angle1, double angle2);
double angleSub(double angle1, double angle2);
double angleCalculate(Line l1, Line l2);
double angleCalculate(Line line);
double angleNormalize(double angle);
bool angleIsBetween(const double compAngle, const double topAngle, const double downAngle);
double angleAverage(double angle1, double angle2);
int toDeg(double angle);
