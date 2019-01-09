#include "stdafx.h"
#include "Polygon.h"
#include "MathFunctions.h"

ConvexPolygon::ConvexPolygon(int numberOfPoints) {
	numOfPoints = numberOfPoints;
	points = new Point[numberOfPoints];
}

ConvexPolygon::~ConvexPolygon() {
	free(points);
}

bool ConvexPolygon::addPoint(const Point & point)
{
	if (firstFreePointId == numOfPoints) return false; // array is full
	printf("addPoint start [%d,%d]\n", point.x, point.y);
	this->print();
	if (firstFreePointId <= 2) { // 2 points in
		printf("addPoint 2 points in\n");
		if (firstFreePointId == 0) {
			middlePoint = point;
		}
		else {
			middlePoint = avgPoint(middlePoint, point, firstFreePointId, 1);
		}
		points[firstFreePointId] = point;
		firstFreePointId++;
		return true;
	}
	if (isInside(point)) {
		printf("addPoint already inside\n");
		return true;	//already inside
	}
	// 3+ points, newPoint is NOT inside
	int minReplace = -1, maxReplace = -1;
	for (int i = 0; i < firstFreePointId-2; i++) {
		Line line(points[i], points[i + 1]);
		if ((line.isAbove(middlePoint, points[i]) and line.isBelow(point, points[i])) or (line.isBelow(middlePoint, points[i]) and line.isAbove(point, points[i]))) { // wrong side, have to replace points
			if (minReplace == -1) minReplace = i;
			maxReplace = i + 1;
		}
	}
	printf("addPoint moveing: %d -> %d\n", minReplace, maxReplace);
	//points should be minReplace -> newPoint -> maxReplace
	if (minReplace != -1) {
		if (minReplace + 1 == maxReplace) { // close, dont need to remove any points, just move them
			for (int i = firstFreePointId - 1; i >= maxReplace; i--) {
				points[i + 1] = points[i];
			}
			points[maxReplace] = point;
			firstFreePointId++;
		}
		else {
			for (int i = maxReplace + 1, j = minReplace + 2; i != j and i < firstFreePointId; i++, j++) {
				points[j] = points[i];
			}
			points[minReplace + 1] = point;
			firstFreePointId = firstFreePointId - (maxReplace - minReplace - 2);
		}
		middlePoint = points[0];
		for (int i = 1; i < firstFreePointId; i++) {
			middlePoint = avgPoint(middlePoint, points[i], i, 1);
		}
	}
	else {
		middlePoint = avgPoint(middlePoint, point, firstFreePointId, 1);
		points[firstFreePointId] = point;
		firstFreePointId++;
	}

	return true;
}

bool ConvexPolygon::isInside(const Point & point)
{
	bool NE = false, SE = false, SW = false, NW = false; // directions
	for (int i = 0; i < firstFreePointId; i++) {
		if (points[i].x < point.x) { // left
			if (points[i].y < point.y) NW = true;
			if (points[i].y > point.y) SW = true;
		}
		if (points[i].x > point.x) { // right
			if (points[i].y < point.y) NE = true;
			if (points[i].y > point.y) SE = true;
		}
	}
	return (NE and SE and NW and SW);
}

void ConvexPolygon::print() {
	printf("ConvexPolygon [%d]:: firstFreePointId: %d\t Mid: [%d,%d]\n\t", numOfPoints, firstFreePointId, middlePoint.x, middlePoint.y);
	for (int i = 0; i < firstFreePointId; i++) printf("[%d,%d] ", points[i].x, points[i].y);
	printf("\n");
}

double ConvexPolygon::volume() {
	double surf = 0;
	for (int i = 0; i < firstFreePointId-1; i++) {
		surf += points[i].x*points[i + 1].y - points[i + 1].x*points[i].y;
	}
	surf += points[firstFreePointId - 1].x*points[0].y - points[0].x*points[firstFreePointId - 1].y;

	return abs(surf / 2);
}


PolygonFigure::PolygonFigure(int numberOfPoints) {
	numOfPoints = numberOfPoints;
	points = new Point[numberOfPoints];
}

PolygonFigure::~PolygonFigure() {
	//delete(points);
}

bool PolygonFigure::addPoint(const Point & point) {
	points[firstFreePointId] = point;
	middlePoint = avgPoint(middlePoint, point, firstFreePointId, 1);
	firstFreePointId++;
	return true;
}

void PolygonFigure::print() {
	printf("Polygon [%d]:: firstFreePointId: %d\t Mid: [%d,%d]\n\t", numOfPoints, firstFreePointId, middlePoint.x, middlePoint.y);
	for (int i = 0; i < firstFreePointId; i++) printf("[%d,%d] ", points[i].x, points[i].y);
	printf("\n");
}

double PolygonFigure::volume() {
	double surf = 0;
	for (int i = 0; i < firstFreePointId - 1; i++) {
		surf += points[i].x*points[i + 1].y - points[i + 1].x*points[i].y;
	}
	surf += points[firstFreePointId - 1].x*points[0].y - points[0].x*points[firstFreePointId - 1].y;

	return abs(surf / 2);
}
