#include "stdafx.h"
#include "Parallelogram.h"

Parallelogram::Parallelogram(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
	//printf("Parallelogram CONST: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;
	lines[0] = Line(points[0], points[1]);
	lines[1] = Line(points[1], points[2]);
	lines[2] = Line(points[2], points[3]);
	lines[3] = Line(points[3], points[0]);
}

Parallelogram::Parallelogram(const ConvexPolygon & poly) {
	Parallelogram bestFig({ 0,0 }, { 0,0 } , { 0,0 }, { 0,0 });
	double bestFigVolume = WIDTH*HEIGHT*2*888888;

	for (int midPoint = 0; midPoint < poly.firstFreePointId; midPoint++) {
		const int prevP = (poly.firstFreePointId + midPoint - 1) % poly.firstFreePointId;
		const int nextP = (poly.firstFreePointId + midPoint + 1) % poly.firstFreePointId;
		Line prevLine(poly.points[midPoint], poly.points[prevP]);
		Line nextLine(poly.points[midPoint], poly.points[nextP]);

		//znajdziemy linie prostopad³¹ do prevLine
		int prevFarPointID = -1;
		double prevFarPointDist = -1;
		for (int i = 0; i < poly.firstFreePointId; i++) {
			const double newDist = prevLine.distanceFrom(poly.points[i]);
			if (newDist > prevFarPointDist) {
				prevFarPointDist = newDist;
				prevFarPointID = i;
			}
		}

		//znajdziemy linie prostopad³¹ do nextLine
		int nextFarPointID = -1;
		double nextFarPointDist = -1;
		for (int i = 0; i < poly.firstFreePointId; i++) {
			const double newDist = nextLine.distanceFrom(poly.points[i]);
			if (newDist > nextFarPointDist) {
				nextFarPointDist = newDist;
				nextFarPointID = i;
			}
		}

		Line prevPerpend = prevLine.getPerpendicularLineViaPoint(poly.points[prevFarPointID]);
		Line nextPerpend = nextLine.getPerpendicularLineViaPoint(poly.points[nextFarPointID]);
		
		//prevLine -> nextLine -> prevPerpend -> nextPerpend
		printf("newFig const\n");
		Parallelogram newFig(prevLine.crossPoint(nextLine), nextLine.crossPoint(prevPerpend), prevPerpend.crossPoint(nextPerpend), nextPerpend.crossPoint(prevLine));
		//printf("newFig midPoint %d\n", midPoint);
		//prevLine.print();
		//nextLine.print();
		//prevPerpend.print();
		//nextPerpend.print();
		printf("newFig\n");
		newFig.print();
		double newVolume = newFig.volume();
		//printf("new fig Volume: %0.1f\n", newVolume);
		if (newVolume < bestFigVolume) {
			bestFig = newFig;
			bestFigVolume = newVolume;
		}
	}
	//printf("Parallelogram commit\n");
	//bestFig.print();
	points[0] = bestFig.points[0];
	points[1] = bestFig.points[1];
	points[2] = bestFig.points[2];
	points[3] = bestFig.points[3];
	lines[0] = Line(points[0], points[1]);
	lines[1] = Line(points[1], points[2]);
	lines[2] = Line(points[2], points[3]);
	lines[3] = Line(points[3], points[0]);
}

Line normalizeLinesToCoverAll(const PolygonFigure& poly, Line baseLine) {
	int farPointID = -1;
	double farPointDist = -1;
	for (int i = 0; i < poly.firstFreePointId; i++) {
		const double newDist = baseLine.distanceFrom(poly.points[i]);
		if (newDist > farPointDist) {
			farPointDist = newDist;
			farPointID = i;
		}
	}

	return baseLine.getPerpendicularLineViaPoint(poly.points[farPointID]);
}

Parallelogram::Parallelogram(const PolygonFigure & poly) {
	Parallelogram bestFig({ 0,0 }, { 0,0 }, { 0,0 }, { 0,0 });
	double bestFigVolume = WIDTH * HEIGHT * 2 ;

	for (int midPoint = 0; midPoint < poly.firstFreePointId; midPoint++) {
		int prevP = (poly.firstFreePointId + midPoint - 1) % poly.firstFreePointId;
		int nextP = (poly.firstFreePointId + midPoint + 1) % poly.firstFreePointId;
		while (prevP != nextP) {
			nextP = (poly.firstFreePointId + midPoint + 1) % poly.firstFreePointId;
			while (prevP != nextP) {
				//printf("midPoint: %d,  Prev: %d,  next: %d\n", midPoint, prevP, nextP);
			

				Line prevLine(poly.points[midPoint], poly.points[prevP]);
				Line nextLine(poly.points[midPoint], poly.points[nextP]);

		/*		//znajdziemy linie prostopad³¹ do prevLine
				int prevFarPointID = -1;
				double prevFarPointDist = -1;
				for (int i = 0; i < poly.firstFreePointId; i++) {
					const double newDist = prevLine.distanceFrom(poly.points[i]);
					if (newDist > prevFarPointDist) {
						prevFarPointDist = newDist;
						prevFarPointID = i;
					}
				}

				//znajdziemy linie prostopad³¹ do nextLine
				int nextFarPointID = -1;
				double nextFarPointDist = -1;
				for (int i = 0; i < poly.firstFreePointId; i++) {
					const double newDist = nextLine.distanceFrom(poly.points[i]);
					if (newDist > nextFarPointDist) {
						nextFarPointDist = newDist;
						nextFarPointID = i;
					}
				}
		*/
				Line prevPerpend = normalizeLinesToCoverAll(poly, prevLine);
				Line nextPerpend = normalizeLinesToCoverAll(poly, nextLine); //nextLine.getPerpendicularLineViaPoint(poly.points[nextFarPointID]);
				prevLine = normalizeLinesToCoverAll(poly, prevPerpend);
				nextLine = normalizeLinesToCoverAll(poly, nextPerpend);
				//lines offset to cover all points



				//prevLine -> nextLine -> prevPerpend -> nextPerpend
				Parallelogram newFig(prevLine.crossPoint(nextLine), nextLine.crossPoint(prevPerpend), prevPerpend.crossPoint(nextPerpend), nextPerpend.crossPoint(prevLine));
				//printf("newFig midPoint %d\n", midPoint);
				//prevLine.print();
				//nextLine.print();
				//prevPerpend.print();
				//nextPerpend.print();
				//printf("newFig\n");
				//newFig.print();
				double newVolume = newFig.volume();
				//printf("new fig Volume: %0.1f\n", newVolume);

				//newFig.print();
				//printf("newVol: %0.1f, bestVol: %0.1f", newVolume, bestFigVolume);
				if (newVolume < bestFigVolume) {
					bestFig = newFig;
					bestFigVolume = newVolume;
				}
				nextP = (poly.firstFreePointId+ nextP + 1) % poly.firstFreePointId;
			}
			nextP = (poly.firstFreePointId + midPoint + 1) % poly.firstFreePointId;
			prevP = (poly.firstFreePointId+ prevP - 1) % poly.firstFreePointId;
		}
		
	}
	//printf("Parallelogram commit\n");
	//bestFig.print();
	points[0] = bestFig.points[0];
	points[1] = bestFig.points[1];
	points[2] = bestFig.points[2];
	points[3] = bestFig.points[3];
	lines[0] = Line(points[0], points[1]);
	lines[1] = Line(points[1], points[2]);
	lines[2] = Line(points[2], points[3]);
	lines[3] = Line(points[3], points[0]);
}



Parallelogram::Parallelogram(const Parallelogram & poly) {
	points[0] = poly.points[0];
	points[1] = poly.points[1];
	points[2] = poly.points[2];
	points[3] = poly.points[3];
	lines[0] = Line(points[0], points[1]);
	lines[1] = Line(points[1], points[2]);
	lines[2] = Line(points[2], points[3]);
	lines[3] = Line(points[3], points[0]);
}

void Parallelogram::addPooint(const Point & point) {
//uselses
}

bool Parallelogram::isInside(const Point & point) {
	const bool line0and2 = (lines[0].isAbove(point, points[1]) and lines[2].isBelow(point, points[3])) or (lines[0].isBelow(point, points[1]) and lines[2].isAbove(point, points[3]));
	const bool line1and3 = (lines[1].isAbove(point, points[0]) and lines[3].isBelow(point, points[2])) or (lines[1].isBelow(point, points[0]) and lines[3].isAbove(point, points[2]));
	return line0and2 and line1and3;
}

void Parallelogram::draw(Frame * frame) {
	for (int i = 0; i < 4; i++) {
		frame->drawLine(points[i], points[(i + 1) % 4], 0xff, 0x0, 0x0);
	}
	//for (int i = 0; i < 4; i++) {
	//	setAllColors(&(frame->content[points[i].y *WIDTH + points[i].x]), 0xff, 0xff, 0x00);
	//}
}

double Parallelogram::volume() {
	return (distance(points[0], points[1]) * lines[0].distanceFrom(points[2]));
}

void Parallelogram::print() {
	printf("Parallelogram:: [%d,%d]  [%d,%d]  [%d,%d]  [%d,%d] V:: %0.2f\n", points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y, points[3].x, points[3].y, this->volume());
	printf("\t"); lines[0].print();
	printf("\t"); lines[1].print();
	printf("\t"); lines[2].print();
	printf("\t"); lines[3].print();
}
