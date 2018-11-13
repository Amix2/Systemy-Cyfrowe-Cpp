#pragma once
#include "stdafx.h"
#include "MathFunctions.h"
#include <math.h>
#define MAX_DISTANCE 2
#define NOT_VALID_ANGLE 88
struct Stripe {
	double angle;
	Point basePoint;
	double topDif, bottomDif;
	double crossDistanceTop, crossDistanceBottom;
	double topPointDistance, bottomPointDistance;

	Stripe initStripe(Point point);

	bool add(Point point);

	double calculateDeltaDis(double newAngle, double crossDistance, double difference);

};