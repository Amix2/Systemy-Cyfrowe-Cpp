#pragma once
#include "stdafx.h"
#include "MathFunctions.h"
#include <math.h>

#define NOT_VALID_ANGLE 88

struct Stripe {

	Point upMax, upMin, downMax, downMin;
	double distUpMax, distUpMin, distDownMax, distDownMin;
	Line lineUp, lineDown;

	Stripe(Point pointStart, Point pointSecound);
	bool add(Point point);
	bool tryAdd(Point point);
	void print();
};

struct Stripe2 {
	double angle;
	DPoint basePoint;
	Point topPoint, bottomPoint;
	DPoint vector;
	double topDif, bottomDif;
	double A = 0, B = 0, C = 0;
	double distanceDivider;

	Stripe2 initStripe(Point point);


	double distancePointToLine(Point point);
	void changeLine(Point point);
	void getLineFromAngle(double angle);
	void initLine(Point secPoint);

	bool isLineReady();

	bool add(Point point);

	void print();

	Stripe2 copy();
	void restore(Stripe2 other);
};