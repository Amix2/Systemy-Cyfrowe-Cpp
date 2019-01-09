#pragma once
#include "stdafx.h"

struct PolygonFigure {
	int numOfPoints;
	int firstFreePointId = 0;
	Point *points;
	Point middlePoint;

	PolygonFigure(int numberOfPoints);
	PolygonFigure() {}
	~PolygonFigure();
	bool addPoint(const Point& point);
	void print();
	double volume();
};
struct ConvexPolygon {
	int numOfPoints;
	int firstFreePointId = 0;
	Point *points;
	Point middlePoint;

	ConvexPolygon(int numberOfPoints);
	~ConvexPolygon();
	bool addPoint(const Point& point);
	bool isInside(const Point& point);
	void print();
	double volume();
};
