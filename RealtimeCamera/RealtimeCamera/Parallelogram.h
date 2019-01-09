#pragma once
#include "stdafx.h"
#include "Line.h"
#include "Polygon.h"
#include "MathFunctions.h"
#include "ImageProcessing.h"

struct Parallelogram {
	Point points[4];
	Line lines[4];
	int pointsSet = 0;

	Parallelogram(const Point& p1, const Point& p2, const Point& p3, const Point& p4);
	Parallelogram(const ConvexPolygon& poly);
	Parallelogram(const PolygonFigure& poly);
	Parallelogram(const Parallelogram& poly);
	Parallelogram() {};
	void addPooint(const Point& point);
	bool isInside(const Point& point);
	void draw(Frame* frame);
	double volume();
	void print();
};