#pragma once
#include "stdafx.h"

struct Line {
	double A, B, C;
	Point point[2];

	Line(const Point& point1, const Point& point2) {
		const double con1 = point1.x - point2.x;
		const double con2 = point1.y - point2.y;
		A = -con2;
		B = con1;
		C = point1.x*con2 - point1.y*con1;
		point[0] = point1;
		point[1] = point2;
	}
	Line(const Point& point1, const double x2, const double y2) {
		const double con1 = point1.x - x2;
		const double con2 = point1.y - y2;
		A = -con2;
		B = con1;
		C = point1.x*con2 - point1.y*con1;
		point[0] = point1;
		point[1] = { (int)(std::round(x2)), (int)(std::round(y2)) };
		//this->print();
	}
	Line() {
		A = B = C = 0;
	}
	Line(int a, int b, int c) {
		A = a;
		B = b;
		C = c;
	}
	Line(const Point& point1, const double angle) {
		const double xOff = std::cos(angle);
		const double yOff = std::sin(angle);
		const double x2 = point1.x + xOff;
		const double y2 = point1.y + yOff;
		const double con1 = point1.x - x2;
		const double con2 = point1.y - y2;
		//printf("LewLine angle: (%f,%f)\n", x2, y2);
		Line(point1, x2, y2);
		A = -con2;
		B = con1;
		C = point1.x*con2 - point1.y * con1;
		point[0] = point1;
		point[1] = { (int)(std::round(x2)), (int)(std::round(y2)) };
	}


	double distanceFrom(const Point& point) {
		return abs(A*point.x + B * point.y + C) / sqrt(A*A + B * B);
	}

	void print() {
		printf("Line on (%d,%d)  (%d,%d):  %.2f *x+ %.2f *y + %.2f = 0\n",
			point[0].x, point[0].y, point[1].x, point[1].y, A, B, C);
	}

	bool isAbove(const Point& referencePoint, const Point& point); // line is Above point
	bool isBelow(const Point& referencePoint, const Point& point); // line is Below point
	double getAngleCloseTo(const double closeAngle); // fing line angle close to set angle
};

void createLineDistanceFromPoint(double outArray[2], const Point& referencePoint, const Point& point, const double distance); // outArray[0] is bigger than outArray[1]
Line createLineDistanceFromPointCloseToPoint(const Point& referencePoint, const Point& point, const double distance, const Point& closePoint);
Line createLineDistanceFromPointCloseToAngle(const Point& referencePoint, const Point& point, const double distance, const double angle);