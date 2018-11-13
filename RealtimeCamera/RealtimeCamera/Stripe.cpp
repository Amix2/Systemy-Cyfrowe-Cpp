#include "stdafx.h"
#include "Stripe.h"

double Stripe::calculateDeltaDis(double newAngle, double crossDistance, double difference) {
	const double oldAngle = std::atan2(difference, crossDistance);
	const double angleDif = angleDifference(angle, newAngle);
	const double fullAngle = oldAngle + angleDif;
	const double pointDistance = std::sqrt(std::pow(crossDistance, 2) + std::pow(difference, 2));
	return std::sin(fullAngle) * pointDistance;
}

bool Stripe::add(Point point) {
	const double pointAngle = angleCalculate(basePoint, point);
	const double pointDistance = distance(basePoint, point); // distance point <-> basePoint
	
	if (angle == NOT_VALID_ANGLE) {
		angle = pointAngle;
		topPointDistance = pointDistance;
		topDif = DBL_MIN;
		crossDistanceTop = pointDistance;
		bottomPointDistance = pointDistance;
		bottomDif = DBL_MIN;
		crossDistanceBottom = pointDistance;
		return true;
	}

	const double angleDif = angleDifference(angle, pointAngle);
	const double pointDfference = std::sin(angleDif) * pointDistance; // distance to current vector
	const int side = angleCompare(angle, pointAngle); // 1-> bottom, -1-> top
	if (side == 1) {
		if (pointDfference > bottomDif) {
			// need to change borders
			if (pointDfference < MAX_DISTANCE) {
				// bottomDif <- pointDfference
				bottomDif = pointDfference;
				crossDistanceBottom = std::cos(angleDif) * pointDistance;
				bottomPointDistance = pointDistance;
				return true;
			}
			else {
				// move angle
				const double newAngleChange = std::asin(MAX_DISTANCE / pointDistance); // newAngle will be as far as possible from new point
				const double newAngle = angleAdd(pointAngle, newAngleChange);
				const double newTopDif = calculateDeltaDis(newAngle, crossDistanceTop, topDif);
				if (newTopDif > MAX_DISTANCE) return false;
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
		if(pointDfference > topDif) {
			// need to change borders
			if (pointDfference < MAX_DISTANCE) {
				// topDif <- pointDfference
				topDif = pointDfference;
				crossDistanceTop = std::cos(angleDif) * pointDistance;
				topPointDistance = pointDistance;
				return true;
			}
			else {
				// move angle
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

Stripe Stripe::initStripe(Point point) {
	return { angle = NOT_VALID_ANGLE ,
		basePoint = point,
		topDif = 0,
		bottomDif = 0,
		crossDistanceTop = 0,
		crossDistanceBottom = 0,
		topPointDistance = 0,
		bottomPointDistance = 0
	};
}