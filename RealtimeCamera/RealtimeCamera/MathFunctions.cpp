#include "stdafx.h"
#include "MathFunctions.h"
#include "Line.h"



Vector initVector() {
	return { NOT_VALID_VCTOR };
}

Vector initVector(Point pStart, Point pEnd) {
	const double x = pEnd.x - pStart.x;
	const double y = pEnd.y - pStart.y;
	if (x == 0)
		if (y > 0)
			return { PI/2 };
		else
			return { -PI/2 };
	return { std::atan2(y, x) };
}

void print(Vector v) {
	printf("Vector: %lf, deg: %lf\n", v.angle, v.angle * 180 / PI);
}

void print(Point p) {
	printf("Point: (%d, %d)\n", p.x, p.y);
}

Vector averageVectorDirections(Vector v1, Vector v2, int w1, int w2) {
	if (v1.angle == NOT_VALID_VCTOR) return v2;
	if (v2.angle == NOT_VALID_VCTOR) return v1;
	const double avg = (v1.angle*w1 + v2.angle*w2) / (double)(w1 + w2);
	if (std::abs(v1.angle - v2.angle) < PI)
		return { avg };

	if ( avg > 0)
		return { avg - PI};
	else
		return { avg + PI };
}

bool isVectorClose(Vector base, Vector comp, double delta) {
	if (comp.angle = NOT_VALID_VCTOR) return false;
	Vector v1;
	if (comp.angle + delta > 2 * PI)
		v1 = { (comp.angle + delta) - 2 * PI };
	else v1 = { comp.angle + delta };

	Vector v2;
	if (comp.angle - delta <= -2 * PI)
		v2 = { (comp.angle - delta) + 2 * PI };
	else v2 = { comp.angle - delta };

	return isBetween(v1, v2, base);
}

bool isBetween(Vector borderMin, Vector borderMax, Vector vec) {
	if (borderMax.angle = NOT_VALID_VCTOR or borderMin.angle == NOT_VALID_VCTOR) return true;
	if(borderMin.angle < borderMax.angle)	return (vec.angle <= borderMax.angle and vec.angle >= borderMin.angle);
	else return (vec.angle >= borderMax.angle and vec.angle <= borderMin.angle);
}

bool isInFrame(Point p) {
	return (p.x >= 0 and p.x < WIDTH and p.y >= 0 and p.y < HEIGHT);
}

double distance(Point p1, Point p2) {
	//if (!isInFrame(p1) or !isInFrame(p2)) return -1;
	return std::sqrt(std::pow((p1.x - p2.x), 2) + std::pow((p1.y - p2.y), 2));
}

int getNextDir(int dir, int step) {
	return (dir + 7 + step) % 8 + 1; // ((dir-1)+8+step)%8 +1
}

int compareDir(int base, int comp) {
	if (comp == base or comp == getNextDir(base, 4)) return 0;
	if (base >= 5) {
		if (comp < base and comp > getNextDir(base, 4)) return -1;
		else return 1;
	}
	else {
		if (comp > base and comp < getNextDir(base, 4)) return 1;
		else return -1;
	}
	return 0;
}

double differenceVector(Vector v1, Vector v2) {
	if (v1.angle == NOT_VALID_VCTOR) return DBL_MAX;
	if (v2.angle == NOT_VALID_VCTOR) return DBL_MAX;
	const double a1 = v1.angle;
	const double a2 = v2.angle;

	if (std::abs(a1 - a2) < PI) { // normal difference
		return std::abs(a1 - a2);
	}
	else {
		return 2*PI - std::abs(a1 - a2);
	}
	
}

int getDirectionFromMove(int move) {
	/*	123				00,10,20
		456		=>		01,11,21
		789				02,12,22	*/
	switch (move) {
	case 00:
		return 1;
		break;
	case 10:
		return 2;
		break;
	case 20:
		return 3;
		break;
	case 01:
		return 4;
		break;
	case 11:
		return 5;
		break;
	case 21:
		return 6;
		break;
	case 02:
		return 7;
		break;
	case 12:
		return 8;
		break;
	case 22:
		return 9;
		break;
	}
}

int getMoveFomDirection(int dir) {
	/*	123				00,10,20
		456		=>		01,11,21
		789				02,12,22	*/
	switch (dir) {
	case 1:
		return 00;
		break;
	case 2:
		return 10;
		break;
	case 3:
		return 20;
		break;
	case 4:
		return 01;
		break;
	case 5:
		return 11;
		break;
	case 6:
		return 21;
		break;
	case 7:
		return 02;
		break;
	case 8:
		return 12;
		break;
	case 9:
		return 22;
		break;
	}
}

int getMoveFromPoints(Point start, Point end) {
	return ((end.x - start.x) + 1) * 10 + ((end.y - start.y) + 1);
}

Point performMoveFromDirection(Point point, int dir) {
	const int move = getMoveFomDirection(dir);
	point.x += (move / 10) - 1;
	point.y += (move % 10) - 1;
	return point;
}

Point neighbours(Point p, int num) {
	/*	123				00,10,20
		456		=>		01,11,21
		789				02,12,22	*/
	int x, y;
	switch (num)
	{
	case 1:
		x = p.x - 1;
		y = p.y - 1;
		if (isInFrame({ x, y })) return { x, y };
		else return p;
		break;
	case 2:
		x = p.x;
		y = p.y - 1;
		if (isInFrame({ x, y })) return { x, y };
		else return p;
		break;
	case 3:
		x = p.x + 1;
		y = p.y - 1;
		if (isInFrame({ x, y })) return { x, y };
		else return p;
		break;
	case 4:
		x = p.x - 1;
		y = p.y;
		if (isInFrame({ x, y })) return { x, y };
		else return p;
		break;
	case 5:
		x = p.x;
		y = p.y;
		if (isInFrame({ x, y })) return { x, y };
		else return p;
		break;
	case 6:
		x = p.x + 1;
		y = p.y;
		if (isInFrame({ x, y })) return { x, y };
		else return p;
		break;
	case 7:
		x = p.x - 1;
		y = p.y + 1;
		if (isInFrame({ x, y })) return { x, y };
		else return p;
	case 8:
		x = p.x;
		y = p.y + 1;
		if (isInFrame({ x, y })) return { x, y };
		else return p;
		break;
	case 9:
		x = p.x + 1;
		y = p.y + 1;
		if (isInFrame({ x, y })) return { x, y };
		else return p;
		break;
	default:
		return p;
		break;
	}
}

// ################################################################
//				LINE

Point findCrossPointLine(const Line& l1, const Line& l2) {
	const double y = (l2.C*l1.A - l1.C*l2.A) / (l1.B*l2.A - l2.B*l1.A);
	const double x = ((-1)*l1.B*y - l1.C) / l1.A;
	return { (int)round(x), (int)round(y) };
}

Line findCrossLine(const Line& l1, const Line& l2) {
	Point edge1, edge2;
	double dist = -1;
	for (int a = 0; a < 2; a++) {
		for (int b = 0; b < 2; b++) {
			if (distance(l1.point[a], l2.point[b]) > dist) {
				edge1 = l1.point[a];
				edge2 = l2.point[b];
			}
		}
	}
	return Line(edge1, edge2);
}
