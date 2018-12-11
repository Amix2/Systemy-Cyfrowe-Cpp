#include "stdafx.h"
#include "Stripe.h"

Stripe::Stripe(Point pointStart, Point pointSecound) {
	upMax = downMax = pointStart;
	upMin = downMin = pointSecound;
	lineUp = lineDown = Line(pointStart, pointSecound);
	distUpMax = distUpMin = distDownMax = distDownMin = 0.0;
}

bool Stripe::add(Point point) {
	const double distNewFromDown = lineDown.distanceFrom(point);
	const double distNewFromUp = lineUp.distanceFrom(point);

	if (distNewFromDown > MAX_DISTANCE) {
		Line newLine(point, downMax);
		if (newLine.distanceFrom(downMin) > MAX_DISTANCE) return false;
		downMin = downMax;
		distDownMin = distDownMax;
		downMax = point;
		distDownMax = distNewFromUp;
		lineDown = newLine;
	}
	else if (distNewFromUp > MAX_DISTANCE) {
		Line newLine(point, upMax);
		if (newLine.distanceFrom(upMin) > MAX_DISTANCE) return false;
		upMin = upMax;
		distUpMin = distUpMax;
		upMax = point;
		distUpMax = distNewFromDown;
		lineUp = newLine;
	}

	if (lineUp.isAbove(upMax, point)) {
		if (distNewFromUp >= distDownMax) {
			downMin = downMax;
			distDownMin = distDownMax;
			downMax = point;
			distDownMax = distNewFromUp;
			lineDown = Line(downMax, downMin);
		}
		else if (distNewFromUp >= distDownMin) {
			downMin = point;
			distDownMin = distNewFromUp;
			lineDown = Line(downMax, downMin);
		}
	}

	if (lineDown.isBelow(downMax, point)) {
		if (distNewFromDown >= distUpMax) {
			upMin = upMax;
			distUpMin = distUpMax;
			upMax = point;
			distUpMax = distNewFromDown;
			lineUp = Line(upMax, upMin);
		}
		else if (distNewFromDown >= distUpMin) {
			upMin = point;
			distUpMin = distNewFromDown;
			lineUp = Line(upMax, upMin);
		}
	}
	return true;
}

bool Stripe::tryAdd(Point point) {
	const double distNewFromDown = lineDown.distanceFrom(point);
	const double distNewFromUp = lineUp.distanceFrom(point);

	if (distNewFromDown > MAX_DISTANCE) {
		Line newLine(point, downMax);
		if (newLine.distanceFrom(downMin) > MAX_DISTANCE) return false;
		downMin = downMax;
		distDownMin = distDownMax;
		downMax = point;
		distDownMax = distNewFromUp;
	}
	else if (distNewFromUp > MAX_DISTANCE) {
		Line newLine(point, upMax);
		if (newLine.distanceFrom(upMin) > MAX_DISTANCE) return false;
		upMin = upMax;
		distUpMin = distUpMax;
		upMax = point;
		distUpMax = distNewFromDown;
	}
	return true;
}


void Stripe::print() {
	printf("\tUpPoints: %d,%d (%lf) | %d,%d (%lf) \n\tDoPoints: %d,%d (%lf) | %d,%d (%lf)\n", 
		upMax.x, upMax.y, distUpMax, upMin.x, upMin.y, distUpMin,
		downMax.x, downMax.y, distDownMax, downMin.x, downMin.y, distDownMin);
}













bool Stripe2::isLineReady() {
	return (A != 0 or B != 0);
}

double Stripe2::distancePointToLine(Point point) {
	if (!isLineReady()) return -1;
	return (std::abs(A*point.x + B * point.y + C) / distanceDivider);
} 


void Stripe2::initLine(Point secPoint) {
	topPoint = secPoint;
	bottomPoint = secPoint;
	const double con1 = secPoint.x - basePoint.x;
	const double con2 = secPoint.y - basePoint.y;
	A = -con2;
	B = con1;
	C = basePoint.x*con2 - basePoint.y*con1;
	vector = { con1, con2 };
	angle = std::atan2(vector.y, vector.x);
	distanceDivider = std::sqrt(std::pow(A, 2) + std::pow(B, 2));
}

void Stripe2::getLineFromAngle(double angle) {
	vector.x = std::cos(angle);
	vector.y = std::sin(angle);
	A = -vector.y;
	B = vector.x;
	C = -A * basePoint.x - B * basePoint.y;
	distanceDivider = std::sqrt(std::pow(A, 2) + std::pow(B, 2));
}

void Stripe2::changeLine(Point point) {
	const double pointDistance = std::sqrt(std::pow((basePoint.x - point.x), 2) + std::pow((basePoint.y - point.y), 2));
	const double pointAngle = angleCalculate(basePoint, point);
	const double minAngle = std::asin(MAX_DISTANCE / pointDistance); // angle from point line to new line
	double lineAngle;

	if (angleCompare(pointAngle, angle)) { // 1 -> below, 2-> up
		lineAngle = angleSub(pointAngle, minAngle);
	}
	else {
		lineAngle = angleAdd(pointAngle, minAngle);
	}

	getLineFromAngle(lineAngle);
	angle = lineAngle;
}

Stripe2 Stripe2::initStripe(Point point) {
	basePoint = { (double)point.x,(double)point.y };
	topDif = 0;
	bottomDif = 0;
	topPoint = { -1, -1 };
	bottomPoint = { -1, -1 };
	A = B = C = 0;
	angle = NOT_VALID_ANGLE;
	distanceDivider = -1;
	vector = { 0, 0 };
	return {};
}

Stripe2 Stripe2::copy() {
	return { angle,
		basePoint,
		topPoint, bottomPoint,
		vector,
		topDif, bottomDif,
		A, B, C,
		distanceDivider
		};
}

void Stripe2::restore(Stripe2 other) {
	angle = other.angle;
	basePoint = other.basePoint;
	topPoint = other.topPoint;
	bottomPoint = other.bottomPoint;
	vector = other.vector;
	topDif = other.topDif;
	bottomDif = other.bottomDif;
	A = other.A;
	B = other.B;
	C = other.C;
	distanceDivider = other.distanceDivider;
}

void Stripe2::print() {
	printf("(%lf, %lf), a: %lf, A: %lf, B: %lf, C: %lf,\t TOP: (%d, %d) dif: %lf,\t BOT: (%d, %d) dif: %lf\t div: %lf \n",
		basePoint.x, basePoint.y, (angle * 180 / PI), A, B, C, topPoint.x, topPoint.y, topDif, bottomPoint.x, bottomPoint.y, bottomDif, distanceDivider);
}

bool Stripe2::add(Point point) {
	if (!isLineReady()) {
		initLine(point);
		return true;
	}
	const double pointAngle = angleCalculate(basePoint, point);
	const double pointDif = distancePointToLine(point);
	const int side = angleCompare(angle, pointAngle); // 1-> bottom, -1-> top
	//printf("pointDif: %lf\n", pointDif);
	//printf("side: %d\n", side);
	if (side == 1) { // BOTTOM
		//printf("BOTTOM\n");
		if (pointDif >= bottomDif) {
			// new bottom point
			if (pointDif > MAX_DISTANCE) {
				//printf("ChangeLine\n");
				Stripe2 copy = this->copy();
				changeLine(point);
				bottomPoint = point;
				bottomDif = distancePointToLine(bottomPoint);
				const double newDist = distancePointToLine(topPoint);
				topDif = newDist;
				//printf("newDist: %lf\n", newDist);
				if (newDist > MAX_DISTANCE) {
					//printf("Stripe2 NO:\n");
					//print();
					restore(copy);
					return false;
				}
				else
					return true;
			}
			else {
				// just change border point
				bottomPoint = point;
				bottomDif = pointDif;
				return true;
			}
		}
		else {
			// Non-Max point
			return true;
		}

	}
	else if (side == -1) { // TOP
		//printf("TOP\n");
		if (pointDif >= topDif) {
			// new bottom point
			if (pointDif > MAX_DISTANCE) {
				Stripe2 copy = this->copy();
				changeLine(point);
				topPoint = point;
				topDif = distancePointToLine(topPoint);
				const double newDist = distancePointToLine(bottomPoint);
				bottomDif = newDist;
				//printf("newDist: %lf\n", newDist);
				//printf("ChangeLine Top: %d, %d : %lf    Bottom:  %d, %d : %lf\n" + topPoint.x, topPoint.y, topDif, bottomPoint.x, bottomPoint.y, bottomDif);
				if (newDist > MAX_DISTANCE) {
					restore(copy);
					return false;
				}
				else
					return true;
			}
			else {
				// just change border point
				topPoint = point;
				topDif = pointDif;
				return true;
			}
		}
		else {
			// Non-Max point
			return true;
		}
	}
	else { // in line -> replace both points if they are in this line (dist == 0)
		if (topDif == 0) topPoint = point;
		if (bottomDif == 0) bottomPoint = point;
	}
	return true;
}

/*
bool Stripe2::add(Point point) {
	const double pointAngle = angleCalculate(basePoint, point);
	const double pointDistance = distance(basePoint, point); // distance point <-> basePoint
	
	if (angle == NOT_VALID_ANGLE) {
		angle = pointAngle;
		topPointDistance = pointDistance;
		topDif = 0;
		crossDistanceTop = pointDistance;
		bottomPointDistance = pointDistance;
		bottomDif = 0;
		crossDistanceBottom = pointDistance;
		printf("Stripe2 Add: NOT_VALID_ANGLE: a: %lf, pointDist: %lf\n", angle, pointDistance);
		return true;
	}
	printf("Stripe2 Add: normal: a: %lf\t", angle);
	const double angleDif = angleDifference(angle, pointAngle);
	const double pointDfference = std::sin(angleDif) * pointDistance; // distance to current vector
	const int side = angleCompare(angle, pointAngle); // 1-> bottom, -1-> top
	if (side == 1) {
		printf(" bottom:\t");
		if (pointDfference > bottomDif) {
			// need to change borders
			printf("Max-Point ");
			if (pointDfference < MAX_DISTANCE) {
				// bottomDif <- pointDfference
				printf("in range ");
				bottomDif = pointDfference;
				crossDistanceBottom = std::cos(angleDif) * pointDistance;
				bottomPointDistance = pointDistance;
				return true;
			}
			else {
				// move angle
				printf("move angle ");
				const double newAngleChange = std::asin(MAX_DISTANCE / pointDistance); // newAngle will be as far as possible from new point
				const double newAngle = angleAdd(pointAngle, newAngleChange);
				const double newTopDif = calculateDeltaDis(newAngle, crossDistanceTop, topDif);
				if (newTopDif > MAX_DISTANCE) return false; // too far to just change angle, we need to move strip sideway
				angle = newAngle; // angle change -> border point does not change (distance from base point) but distance from line does
				topDif = newTopDif;
				crossDistanceTop = std::sqrt(std::pow(topPointDistance, 2) - std::pow(topDif, 2));
				return true;
			}
		}
		else {
			// non-max point -> can add
			return true;
		}
	}
	else if (side == -1) {
		printf(" top:\t");
		if(pointDfference > topDif) {
			printf("Max-Point ");
			// need to change borders
			if (pointDfference < MAX_DISTANCE) {
				// topDif <- pointDfference
				printf("in range ");
				topDif = pointDfference;
				crossDistanceTop = std::cos(angleDif) * pointDistance;
				topPointDistance = pointDistance;
				return true;
			}
			else {
				// move angle
				printf("move angle ");
				const double newAngleChange = std::asin(MAX_DISTANCE / pointDistance); // newAngle will be as far as possible from new point
				const double newAngle = angleSub(pointAngle, newAngleChange);
				const double newBottonDif = calculateDeltaDis(newAngle, crossDistanceBottom, bottomDif);
				if (newBottonDif > MAX_DISTANCE) return false;
				angle = newAngle; // angle change -> border point does not change (distance from base point) but distance from line does
				bottomDif = newBottonDif;
				crossDistanceBottom = std::sqrt(std::pow(bottomPointDistance, 2) - std::pow(bottomDif, 2));
				return true;
			}
		}
		else {
		// non-max point -> can add
		return true;
		}
	}
	else return true;
}

*/
