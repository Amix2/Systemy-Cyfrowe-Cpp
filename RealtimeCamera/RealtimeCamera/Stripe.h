#pragma once
#include "stdafx.h"
#include "MathFunctions.h"
#include "Line.h" 
#include <math.h>

#define NOT_VALID_ANGLE 88

struct Stripe {
	Point basePoint;
	Point topPoint, downPoint;
	double topAngle, downAngle;
	Line line;
	//Line lineTop, lineDown;
	double MaxDistance;
	double score;

	Stripe(double MaxDistance);
	Stripe(Point point, double MaxDistance);
	Stripe(Point p1, Point p2, double MaxDistance);

	bool tryAdd(const Point& point);
	void add(const Point& point);

	void print();
};

struct Stripe3 {

	Point upMax, upMin, downMax, downMin;
	double distUpMax, distUpMin, distDownMax, distDownMin;
	Line lineUp, lineDown;

	Stripe3(Point pointStart, Point pointSecound);
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

	void initStripe(Point point);

	Stripe2() {}
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