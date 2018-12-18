#include "stdafx.h"
#include "Line.h"
#include "MathFunctions.h"

bool Line::isBelow(const Point& referencePoint, const Point& point) {
	const double anglePoint = angleCalculate(referencePoint, point);
	const double angleLine = getAngleCloseTo(anglePoint);
	const int cmp = angleCompare(anglePoint, angleLine); // this > compLine
	return cmp >= 0;
}


bool Line::isAbove(const Point& referencePoint, const Point& point) {  // line is Above point
	const double anglePoint = angleCalculate(referencePoint, point);
	const double angleLine = getAngleCloseTo(anglePoint);
	const int cmp = angleCompare(anglePoint, angleLine); // anglePoint > angleLine
	return cmp <= 0;
}

double Line::getAngleCloseTo(const double closeAngle) {
	const double angle = angleCalculate(*this);
	if (angleDifference(angle, closeAngle) < PI/2) return angleNormalize(angle);
	else	return angleNormalize(angle - PI);

}

void createLineDistanceFromPoint(double outArray[2], const Point& referencePoint, const Point& point, const double distanceFromPoint) {
	//printf("createLineDistanceFromPoint: from %d,%d to %d,%d dist: %f\n", referencePoint.x, referencePoint.y, point.x, point.y, distanceFromPoint);

	const double pointAngle = angleCalculate(referencePoint, point);
	const double pointDistance = distance(referencePoint, point);
	const double newPointDist = std::sqrt(std::pow(pointDistance, 2) - std::pow(distanceFromPoint, 2));
	//printf("pointAngle %f, pointDistance %f, newPointDist %f\n", pointAngle, pointDistance, newPointDist);
	if (newPointDist != 0) {
		const double angleDif = std::atan2(distanceFromPoint, newPointDist);
		outArray[0] = angleAdd(pointAngle, angleDif);
		outArray[1] = angleSub(pointAngle, angleDif);
	}
	else {
		outArray[0] = angleAdd(pointAngle, PI/2);
		outArray[1] = angleSub(pointAngle, PI/2);
	}
}

Line createLineDistanceFromPointCloseToPoint(const Point& referencePoint, const Point& point, const double distance, const Point& closePoint) {
	//printf("createLineDistanceFromPointCloseToPoint: from %d,%d to %d,%d dist: %f\n", referencePoint.x, referencePoint.y, point.x, point.y, distance);
	double angleArr[2];
	createLineDistanceFromPoint(angleArr, referencePoint, point, distance);
	//printf("createLineDistanceFromPointCloseToPoint angleArr: ");
	//for (int i = 0; i < 2; i++)
	//	printf("%lf  ", angleArr[i]);
	//printf("\n");

	const double pointAngle = angleCalculate(referencePoint, point);
	const double angleDif1 = angleDifference(pointAngle, angleArr[0]);
	const double angleDif2 = angleDifference(pointAngle, angleArr[1]);
	if (angleDif1 < angleDif2) return Line(referencePoint, angleArr[0]);
	return Line(referencePoint, angleArr[1]);
}

Line createLineDistanceFromPointCloseToAngle(const Point& referencePoint, const Point& point, const double distance, const double pointAngle) {
	//printf("createLineDistanceFromPointCloseToAngle: from %d,%d to %d,%d dist: %f\n", referencePoint.x, referencePoint.y, point.x, point.y, distance);

	double angleArr[2];
	createLineDistanceFromPoint(angleArr, referencePoint, point, distance);
	//printf("createLineDistanceFromPointCloseToAngle angleArr: ");
	//for (int i = 0; i < 2; i++)
	//	printf("%lf  ", angleArr[i]);
	//printf("\n");

	const double angleDif1 = angleDifference(pointAngle, angleArr[0]);
	const double angleDif2 = angleDifference(pointAngle, angleArr[1]);
	if (angleDif1 < angleDif2) return Line(referencePoint, angleArr[0]);
	return Line(referencePoint, angleArr[1]);
}

/*
void createLineDistanceFromPoint(double outArray[4], const Point& referencePoint, const Point& point, const double distance) {
double v1[2], v2[2];
//double newPointVec1[2], newPointVec2[2];
int vector[2];
vector[0] = point.x - referencePoint.x;
vector[1] = point.y - referencePoint.y;
printf("Vector: %d, %d\n", vector[0], vector[1]);
// liczenie wektora prostopad³ego o d³ugoœci d
const double b2 = (std::pow(distance, 2) * std::pow(vector[0], 2)) / (std::pow(vector[0], 2) + std::pow(vector[1], 2));
printf("b2: %lf\n", b2);
v1[0] = std::sqrt(std::pow(distance, 2) - b2);
v1[1] = -std::sqrt(b2);
v2[0] = -std::sqrt(std::pow(distance, 2) - b2);
v2[1] = std::sqrt(b2);
printf("%lf, %lf\n%lf,%lf\n", v1[0],v1[1],v2[0], v2[1]);
const double newPoint1[2] = { vector[0] + v1[0] + referencePoint.x,	vector[1] + v1[1] + referencePoint.y };
const double newPoint2[2] = { vector[0] + v2[0] + referencePoint.x,	vector[1] + v2[1] + referencePoint.y };

outArray[0] = newPoint1[0];
outArray[1] = newPoint1[1];
outArray[2] = newPoint2[0];
outArray[3] = newPoint2[1];
}

Line createLineDistanceFromPointCloseToPoint(const Point& referencePoint, const Point& point, const double distance, const Point& closePoint) {
double pointArray[4];
createLineDistanceFromPoint(pointArray, referencePoint, point, distance);
const double dist1 = (std::pow(closePoint.x - pointArray[0], 2) + std::pow(closePoint.y - pointArray[1], 2));
const double dist2 = (std::pow(closePoint.x - pointArray[3], 2) + std::pow(closePoint.y - pointArray[4], 2));
for (int i = 0; i < 4; i++)
printf("%lf  ", pointArray[i]);
if (dist1 > dist2) return Line(referencePoint, pointArray[0], pointArray[1]);
return Line(referencePoint, pointArray[2], pointArray[3]);
}

*/