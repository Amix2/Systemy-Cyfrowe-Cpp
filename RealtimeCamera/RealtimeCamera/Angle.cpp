#include "stdafx.h"
#include "Angle.h"

double angleDifference(double angle1, double angle2) {
	const double angleDif = std::abs(angle1 - angle2);
	if (angleDif < PI)	return angleDif;
	else	return 2 * PI - angleDif;
}

int angleCompare(double angle1, double angle2) { // angle1 > angle2
	if (std::abs(angle1 - angle2) <= ZERO_ANGLE) return 0;
	if (std::abs(angle1 - angle2) < PI) {
		if (angle1 > angle2) return 1;
		if (angle1 < angle2) return -1;
		return 0;
	}
	if (angle1 < angle2) return 1;
	if (angle1 > angle2) return -1;
	return 0;
}

double angleCalculate(Point start, Point end) {
	const double x = end.x - start.x;
	const double y = end.y - start.y;
	if (x == 0)
		if (y > 0)
			return { PI / 2 };
		else
			return { -PI / 2 };
	return std::atan2(y, x);
}
double angleCalculate(DPoint start, Point end) {
	const double x = end.x - start.x;
	const double y = end.y - start.y;
	if (x == 0)
		if (y > 0)
			return { PI / 2 };
		else
			return { -PI / 2 };
	return std::atan2(y, x);
}

double angleAdd(double angle1, double angle2) {
	const double out = angle1 + angle2;
	if (out > PI) return out - 2 * PI;
	return out;
}
double angleSub(double angle1, double angle2) {
	const double out = angle1 - angle2;
	if (out < -PI) return out + 2 * PI;
	return out;
}

double angleCalculate(Line l1, Line l2) {
	const double ang1 = atan2(-l1.A, l1.B);
	const double ang2 = atan2(-l2.A, l2.B);
	const double angleDif = angleDifference(std::max(ang1, ang2), std::min(ang1, ang2));
	return std::min(angleDif, PI - angleDif);
}

double angleCalculate(Line line) {
	const double angle = atan2(-line.A, line.B);
	if (angle > PI) return angle - PI;
	if (angle <= -PI) return angle + PI;
	return angleNormalize(angle);
}

int toDeg(double angle) {
	return (int)(angle * 180 / PI);
}

double angleNormalize(double angle) {
	while (angle <= -PI) angle += 2 * PI;
	while (angle > PI) angle -= 2 * PI;
	return angle;
}

bool angleIsBetween(const double compAngle, const double topAngle, const double downAngle) {
	//printf("angles: %f, %f, %f\n", compAngle, topAngle, downAngle);
	//printf("angleIsBetween: %d : %d\n", angleCompare(compAngle, topAngle), angleCompare(compAngle, downAngle));
	return (angleCompare(compAngle, topAngle) <= 0) and (angleCompare(compAngle, downAngle) >= 0);
}

double angleAverage(double angle1, double angle2) {
	if (std::abs(angle1 - angle2) < PI) {
		return (angle1 + angle2) / 2;
	}
	return angleNormalize(((angle1 + angle2) / 2) + PI);
}