#include "stdafx.h"
#include "LineFinder.h"
#include "ScreenHandler.h"
#include "Queue.h"
#include <windows.h>

Screen screen2;


//Point traceLineOneSide(Frame frame, Point startPoint, Point secPoint, bool markVisited, Point* lastPoint, Stripe2* stripe);

// IN: point; OUT: 2close points filled, ret DIrection
//int isGodStartingPoint(Frame frame, Point point);

// IN: point; OUT: endPoint == {-1,-1} if not found 
//Point findNextBestStartingPoint(Frame frame, const Point point, int dir);

// IN: 2 points; OUT: endPoint of line AND Stripe*
//Point traceLine(Frame frame, Point startPoint, Point secPoint, Stripe* stripe, bool markVisited = false);

// IN: 1 punkt, OUT: koniec lini z tego punktu
//Point findEndOfContinueLine(Frame frame, Point point);

void setVisited(int* pp) {
	setAlpha(pp, 11);
}

bool isVisited(int pp) {
	return getAlpha(pp) == 11;
}

bool isEmpty(int pp) {
	return getRed(pp) == 0;
}

void clearPixel(int* pp) {
	setAllColors(pp, 0, 0, 0);
}



void findLanes(Frame frame) {
	//initScreen(WIDTH * 20, HEIGHT * 20, &screen2);
	//Sleep(1000);
	PointQueue queue(6);

	int cord;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			// in LOOP: check queue, check isGoodPoint(), startlineSearch and add to QUEUE
			cord = y * WIDTH + x;
			
			while (!queue.isEmpty()) {
				Point point = queue.take();
				Point endPoint = findEndOfContinueLine(frame, point);
				if (!(point == endPoint)) {
					//printf("LINE: (%d,%d) === (%d,%d)\n", point.x, point.y, endPoint.x, endPoint.y);
					addVertexPair(point, endPoint);
					queue.add(point);
					queue.add(endPoint);
				}
			}

			if (!isEmpty(frame[cord]) and !isVisited(frame[cord])) {
				const int startDir = isGodStartingPoint(frame, { x, y });
				if (startDir != -1) {
					Point startPoint = findNextBestStartingPoint(frame, { x, y }, startDir);
					//printf("findLanes::startPoint : %d, %d\n", startPoint.x, startPoint.y);
					queue.add(startPoint);
				}
			}
		}
	}
	while (!queue.isEmpty()) {
		Point point = queue.take();
		Point endPoint = findEndOfContinueLine(frame, point);
		if (!(point == endPoint)) {
			//printf("LINE: (%d,%d) === (%d,%d)", point.x, point.y, endPoint.x, endPoint.y);
			queue.add(point);
			queue.add(endPoint);
		}
	}
}

int isGodStartingPoint(Frame frame, Point point) {
	bool neiTab[9] = { false, false, false, false, false, false, false, false, false };
	int dir1 = -1, dir2 = -1;
	for(int i = 1; i <= 9; i++) {
		Point nei = neighbours(point, i);
		if (getRed(frame[nei]) > 0) {
			neiTab[i - 1] = true;
		}
		//printf("%d ", neiTab[i - 1]);
	}
	//printf("\n");

	if (neiTab[7 - 1] and neiTab[3 - 1]) {
		dir1 = 7;
		dir2 = 3;
	}
	else if (neiTab[8 - 1] and neiTab[2 - 1]) {
		dir1 = 8;
		dir2 = 2;
	}
	else if (neiTab[9 - 1] and neiTab[1 - 1]) {
		dir1 = 9;
		dir2 = 1;
	}
	else if (neiTab[6 - 1] and neiTab[4 - 1]) {
		dir1 = 6;
		dir2 = 4;
	}
	else {
		return -1;
	}
	return dir1 * 10 + dir2;
}

Point findNextBestStartingPoint(Frame frame, const Point point, int dir) {
	const Point secPoint1 = performMoveFromDirection(point, dir/10);
	Stripe stripe1(point, secPoint1, MAX_DISTANCE);
	const Point endPoint1 = traceLine(frame, point, secPoint1, &stripe1, false);

	const Point secPoint2 = performMoveFromDirection(point, dir % 10);
	Stripe stripe2(point, secPoint2, MAX_DISTANCE);
	const Point endPoint2 = traceLine(frame, point, secPoint2, &stripe2, false);

	//printf("findNextBestStartingPoint 2p dir: %d  from: (%d,%d), (%d,%d)\n", dir, secPoint1.x, secPoint1.y, secPoint2.x, secPoint2.y);
	//printf("findNextBestStartingPoint 2p end: (%d,%d), (%d,%d)\n", endPoint1.x, endPoint1.y, endPoint2.x, endPoint2.y);
	if (stripe1.score > stripe2.score) return endPoint2;
	if (stripe1.score < stripe2.score) return endPoint1;
	if (distance(point, endPoint1) > distance(point, endPoint2)) return endPoint1;
	return endPoint2;
}

Point findEndOfContinueLine(Frame frame, Point point) {
	Point out = point;
	for (int i = 1; i <= 9; i++) {
		Point nei = neighbours(point, i);
		if (!isEmpty(frame[nei]) and !isVisited(frame[nei]) and !(nei == point)) {

			out = traceLine(frame, point, nei, NULL, true);
			setVisited(&(frame[out]));
			break;
		}
	}
	return out;
}

Point traceLine(Frame frame, Point startPoint, Point secPoint, Stripe* str, bool markVisited) {
	// finds point at the end of line, setVisited to every point without starting id markVisited flag is set
	Stripe* stripe;
	if (str == NULL)
		stripe = new Stripe(startPoint, secPoint, MAX_DISTANCE);
	else
		stripe = str;

	Point currentPoint;
	double currentDistance;
	bool foundPoint = true; // true -> change current point

	int checkOrder[] = { 2, 4, 6, 8, 1, 3, 7, 9 };
	const int checkNum = 8;
	Point nextPoint = secPoint;
	while (foundPoint) {
		// main Loop: search close neighbours first
		currentPoint = nextPoint;
		currentDistance = distance(startPoint, currentPoint);
		stripe->add(currentPoint);
		//printf("traceLine from (%d,%d), curr: (%d,%d)\n", startPoint.x, startPoint.y, currentPoint.x, currentPoint.y);

		double nextPointDistance = -1;
		foundPoint = false;
		int neiFoundNum = checkNum;

		for (int i = 0; i < checkNum; i++) { // find nextPoint
			const int neiNum = checkOrder[i];
			const Point nei = neighbours(currentPoint, neiNum);
			if (isEmpty(frame[nei]) or isVisited(frame[nei])) continue; // first quick check: empty or visited OUT

			const double neiDistance = distance(startPoint, nei);
			if (neiDistance <= currentDistance or neiDistance <= nextPointDistance) continue; // sec check: is closer than diferent next or current

			// now: is better than prev nextPoint, is good distance, not visited
			if (stripe->tryAdd(nei)) {
				nextPoint = nei;	// change NextPoint
				nextPointDistance = neiDistance;
				foundPoint = true;
			}

			if (neiNum == 8 and foundPoint) {
				break; // we have best close point, end search
				neiFoundNum = 4;
			}
		} // inner Loop end

		for (int i = 0; foundPoint and markVisited and i < neiFoundNum; i++) {
			setVisited(&(frame[neighbours(currentPoint, i)]));
		}
	}

	// currentPoint is the last point is Stripe
	return currentPoint;

}

Point traceLine2(Frame frame, Point startPoint, Point secPoint, Stripe* str, bool markVisited) {
	// finds point at the end of line, setVisited to every point without starting id markVisited flag is set
	Stripe* stripe;
	if (str == NULL)
		stripe = new Stripe(startPoint, secPoint, MAX_DISTANCE);
	else
		stripe = str;

	Stripe bigStripe(startPoint, secPoint, 2*MAX_DISTANCE);
	PointQueue queue(0);

	Point currentPoint;
	double currentDistance;
	Point endPoint;
	double endPointDistance;
	bool foundPoint = true; // true -> change current point
	bool foundPossiblePoint = true;

	int checkOrder[] = { 2, 4, 6, 8, 1, 3, 7, 9 };
	const int checkNum = 8;
	Point nextPoint = secPoint;
	Point possiblePoint = secPoint;
	while (foundPoint) { //  or (foundPossiblePoint and !queue.isFull())
		// main Loop: search close neighbours first
		stripe->add(currentPoint);
		bigStripe.add(currentPoint);

		if (foundPoint) {
			currentPoint = nextPoint;
			currentDistance = distance(startPoint, currentPoint);
			endPoint = currentPoint;
			endPointDistance = currentDistance;
		}
		else if (foundPossiblePoint) {
			currentPoint = possiblePoint;
			currentDistance = distance(startPoint, currentPoint);
		}
		//printf("traceLine from (%d,%d), curr: (%d,%d)\n", startPoint.x, startPoint.y, currentPoint.x, currentPoint.y);

		double nextPointDistance = -1;
		double possiblePointDistance = -1;
		foundPoint = false;
		foundPossiblePoint = false;
		int neiFoundNum = checkNum;

		for (int i = 0; i < checkNum; i++) { // find nextPoint
			const int neiNum = checkOrder[i];
			const Point nei = neighbours(currentPoint, neiNum);
			if (isEmpty(frame[nei]) or isVisited(frame[nei])) continue; // first quick check: empty or visited OUT

			const double neiDistance = distance(startPoint, nei);
			if (neiDistance <= currentDistance) continue; // sec check: is closer than current

			// now: is better than prev nextPoint, is good distance, not visited
			if (stripe->tryAdd(nei) and neiDistance > nextPointDistance) {
				nextPoint = nei;	// change NextPoint
				nextPointDistance = neiDistance;
				foundPoint = true;
				foundPossiblePoint = true;
			}

			// point is in bigger stripe
			if (false and bigStripe.tryAdd(nei) and neiDistance > possiblePointDistance) {
				possiblePoint = nei;
				possiblePointDistance = neiDistance;
				foundPossiblePoint = true;
			}

			if (neiNum == 8 and ( foundPoint or foundPossiblePoint )) {
				neiFoundNum = 4;
				break; // we have best close point, end search
			}
		} // inner Loop end

		// add points to queue if is in possible stripe
		for (int i = 0; false and !queue.isFull() and !foundPoint and foundPossiblePoint and markVisited and i < neiFoundNum; i++) {
			queue.add(neighbours(currentPoint, i));
		}

		// mark points in best stripe
		for (int i = 0; foundPoint and markVisited and i < neiFoundNum; i++) {
			setVisited(&(frame[neighbours(currentPoint, i)]));
		}

		// mark points from queue when back in best stripe
		while (false and foundPoint and markVisited and !queue.isEmpty()) {
			setVisited(&(frame[queue.take()]));
		}
	}

	// currentPoint is the last point is Stripe
	return endPoint;

}

/*
void addOneLine(Frame frame, Point startPoint, bool continuation) {
	bool neiTab[9] = {false, false, false, false, false, false, false, false, false};
	Point lastP1, lastP2, p1, p2;
	Stripe2 str1, str2;
	int dir1 = -1, dir2 = -1;
	if (!continuation) {
		for (int i = 1; i <= 9; i++) {
			Point nei = neighbours(startPoint, i);
			if (!(nei == startPoint) and getRed(frame[nei]) > 0) {
				neiTab[i - 1] = true;
			}
		}

		if (neiTab[7 - 1] and neiTab[3 - 1]) {
			dir1 = 7;
			dir2 = 3;
		}
		else if (neiTab[8 - 1] and neiTab[2 - 1]) {
			dir1 = 8;
			dir2 = 2;
		}
		else if (neiTab[9 - 1] and neiTab[1 - 1]) {
			dir1 = 9;
			dir2 = 1;
		}
		else if (neiTab[6 - 1] and neiTab[4 - 1]) {
			dir1 = 6;
			dir2 = 4;
		}
		else {
			return;
		}
		Point lastP1, lastP2;
		Stripe2 str1, str2;
		//printf("\nP1\n");
		p1 = traceLineOneSide(frame, startPoint, neighbours(startPoint, dir1), false, &lastP1, &str1);
		//printf("\nP2\n");
		p2 = traceLineOneSide(frame, startPoint, neighbours(startPoint, dir2), false, &lastP2, &str2);
		if (str1.topDif + str1.topDif < str2.topDif + str2.topDif) {
			p2 = traceLineOneSide(frame, p1, lastP1, true, &lastP2, &str2);
		}
		else {
			p1 = traceLineOneSide(frame, p2, lastP2, true, &lastP1, &str1);
		}
	}
	else { // continuation
		p1 = startPoint;
		for (int i = 1; i <= 9; i++) {
			Point nei = neighbours(startPoint, i);
			if (!(nei == startPoint) and getRed(frame[nei]) > 0) {
				p2 = nei;
				break;
			}
		}
		p2 = traceLineOneSide(frame, p1, p2, true, &lastP2, &str2);
	}

	//setAllColors(&(frame[p1]), 0xff, 0, 0);
	//setAllColors(&(frame[p2]), 0, 0, 0xff);
	//printf("\n\nNext  run %d, %d -> %d, %d ::: %d, %d -> %d, %d\n", p1.x, p1.y, lastP1.x, lastP1.y, p2.x, p2.y, lastP2.x, lastP2.y);
	//if (distance(startPoint, p1) > distance(startPoint, p2)) {


	//printf("\nLINE: (%d, %d), dir: %d/%d : (%d, %d) -> (%d, %d)\n\n", startPoint.x, startPoint.y, dir1, dir2, p1.x, p1.y, p2.x, p2.y);
	if (isInFrame(p1) and isInFrame(p2) and distance(p1, p2) > minLineLenght) {
		addVertexPair(p1.x, p1.y, p2.x, p2.y);
	}
	//addOneLine(frame, p1);
	//addOneLine(frame, p2);
}

Point traceLineOneSide(Frame frame, Point startPoint, Point secPoint, bool markVisited, Point* lastPoint, Stripe2* stripe) {
	const int maxState = 0; // -> max acceptable state

	//Stripe stripe(startPoint, secPoint);
	stripe->initStripe(startPoint);
	stripe->add(secPoint);

	bool foundPoint = true;
	bool lastPointSet = false;
	double lastPointDistance = 0;
	*lastPoint = startPoint;
	Point currentPoint = secPoint;
	double currentPointDistance = distance(currentPoint, secPoint);
	Point nextPoint = secPoint;
	double nextPointDistance = -1;
	Point bestPoint = secPoint;
	int state = 0; // 0-> w lini | 1-MAX -> warning

	while (foundPoint) {
		//printf("startPoint: %d, %d,\t nextPoint: %d, %d,\t currentPointDistance: %lf\n", startPoint.x, startPoint.y, nextPoint.x, nextPoint.y, currentPointDistance);
		//stripe->add(currentPoint);
		//stripe.print();
		//Sleep(500);
		//renderFrame(frame, screen2);

		foundPoint = false;
		for (int i = 2; i <= 9; i+=2) { // InlineLoop
			const Point nei = neighbours(currentPoint, i);
			const double dist = distance(startPoint, nei);
			// check all neibours

			if (isInFrame(nei) and !isEmpty(frame[nei]) and !isVisited(frame[nei]) and dist > currentPointDistance and stripe->add(nei)) {
				// in frane, edge, not visited, not back, in stripe
				//printf("N: %d, %d: %lf ", nei.x, nei.y, dist);
				foundPoint = true;

				if (dist > nextPointDistance) { // new better next point
					//printf("<-XXX   ");
					nextPoint = nei;
					nextPointDistance = dist;
				}
			}

		} // end InlineLoop neibours loop
		if (foundPoint) {
			for (int i = 2; markVisited and i <= 9; i+=2) {
				const Point nei = neighbours(currentPoint, i);
				const double dist = distance(startPoint, nei);
				if (isInFrame(nei) and !isEmpty(frame[nei]) and !isVisited(frame[nei]) and dist > currentPointDistance) {
					//printf("I ");
					setVisited(&(frame[nei]));
					//setAllColors(&(frame[nei]), mask, mask, 0);
				}
			}
		}
		else {
			for (int i = 1; i <= 9; i += 2) { // CrossLoop
				const Point nei = neighbours(currentPoint, i);
				const double dist = distance(startPoint, nei);
				// check all neibours


				if (isInFrame(nei) and !isEmpty(frame[nei]) and !isVisited(frame[nei]) and dist > currentPointDistance and stripe->add(nei)) {
					// in frane, edge, not visited, not back, in stripe
					//printf("N: %d, %d: %lf ", nei.x, nei.y, dist);
					foundPoint = true;

					if (dist > nextPointDistance) { // new better next point
													//printf("<-XXX   ");
						nextPoint = nei;
						nextPointDistance = dist;
					}
				}

			} // end CrossLoop loop
			if (foundPoint) {
				for (int i = 1; markVisited and i <= 9; i ++) {
					const Point nei = neighbours(currentPoint, i);
					const double dist = distance(startPoint, nei);
					if (isInFrame(nei) and !isEmpty(frame[nei]) and !isVisited(frame[nei]) and dist > currentPointDistance) {
						//printf("I ");
						setVisited(&(frame[nei]));
						//setAllColors(&(frame[nei]), mask, mask, 0);
					}
				}
			}
		}
		// nextPoint -> furthest neibour, InLine are better
		if (foundPoint) {
			//printf("YES: %d\n", state);
			
			state = max(state - 1, 0); // state --
			*lastPoint = currentPoint;
			currentPoint = nextPoint;
			currentPointDistance = nextPointDistance;
			if(state == 0) bestPoint = currentPoint;


		}
		else { // not found, state ++
			//printf("NOT: %d\n", state);
			state++;
			currentPoint = nextPoint;
			currentPointDistance = nextPointDistance;
			//if (state <= maxState) foundPoint = true;
		}
		nextPointDistance = -1;
		//printf("\n");
	}
	return bestPoint;
}
Point traceLineOneSide2(Frame frame, Point startPoint, Point secPoint, uint8_t mask, Point* lastPoint, Stripe2* stripe) {
	const int maxState = 0; // -> max acceptable state

	//stripe->initStripe(startPoint, secPoint);
	//stripe->add(secPoint);

	bool foundPoint = true;
	bool continueSearch = true;
	double lastPointDistance = 0;
	*lastPoint = startPoint;
	Point currentPoint = secPoint;
	double currentPointDistance = distance(startPoint, secPoint);
	Point nextPoint = secPoint;
	double nextPointDistance = distance(startPoint, secPoint);
	Point bestPoint = secPoint;
	int state = 0; // 0-> w lini | 1-MAX -> warning
	//Sleep(1000);
	while (continueSearch and state <= maxState) {
		//printf("startPoint: %d, %d,\t nextPoint: %d, %d,\t currentPointDistance: %lf\n", startPoint.x, startPoint.y, nextPoint.x, nextPoint.y, currentPointDistance);
		//stripe->print();
		//Sleep(500);
		//renderFrame(frame, screen2);

		foundPoint = false;
		continueSearch = false;
		for (int i = 1; i <= 9; i++) {
			Point nei = neighbours(currentPoint, i);
			const double dist = distance(startPoint, nei);
			// check all neibours
			// point in stripe exists::	counting only on stripe: nextPoint - best dist
			// not ^ :: nextPoint - best dist; foundPoint = false
			//printf("N: %d, %d: %lf (%d)", nei.x, nei.y, dist, isVisited(frame[nei], 0xff));
			if (isInFrame(nei) and !isEmpty(frame[nei]) and !isVisited(frame[nei]) and dist > currentPointDistance) {
				// in frane, not visited, not back
				// isInStripe and foundPoint -> foundPoint = true, continueSearch = true, check if is the best
				// isInStripe and not foundPoint -> foundPoint = true, continueSearch = true, set next best
				// not isInStripe and foundPoint -> continueSearch - the same
				// not isInStripe and not foundPoint -> continueSearch = true, check if is the best
				const bool isInStripe = stripe->add(nei);
				const bool isBetterNext = (dist > nextPointDistance);
				//printf("II  ");
				if (isInStripe and foundPoint) {
					if (isBetterNext) {
						nextPoint = nei;
						nextPointDistance = dist;
					}
				}
				else if (!isInStripe and !foundPoint) {
					continueSearch = true;
					if (isBetterNext) {
						nextPoint = nei;
						nextPointDistance = dist;
					}
				}
				else if (isInStripe and !foundPoint) {
					foundPoint = true;
					continueSearch = true;
					nextPoint = nei;
					nextPointDistance = dist;
				}
			}
		} // end neibours loop
		  // nextPoint -> furthest neibour
		if (foundPoint) {
			//printf("YES: %d\t", state);
			for (int i = 1; i <= 9; i++) {
				const Point nei = neighbours(currentPoint, i);
				const double dist = distance(startPoint, nei);
				if (isInFrame(nei) and !isEmpty(frame[nei]) and !isVisited(frame[nei]) and dist > currentPointDistance) {
					//printf("I ");
					setVisited(&(frame[nei]));
					//setAllColors(&(frame[nei]), mask, mask, 0);
				}
			}
			//printf("\n");
			state = 0; // state --
			*lastPoint = currentPoint;	// set output
			bestPoint = nextPoint;		// ^^^^
			currentPoint = nextPoint;	// set nextPoint
			currentPointDistance = nextPointDistance;
		}
		else if (continueSearch) {
			state++;
			currentPoint = nextPoint;
			currentPointDistance = nextPointDistance;
		}
		//setAllColors(&(frame[currentPoint]), mask, mask, 0);
		nextPointDistance = -1;
		//printf("\n");
	}
	return bestPoint;
}
*/
/*

void setVisited(int* pp) {
	setAlpha(pp, 0xff);
}

bool isVisited(int pp) {
	return getAlpha(pp) == 0xff;
}

bool isEmpty(int pp) {
	return getRed(pp) == 0;
}

void clearPixel(int* pp) {
	setAllColors(pp, 0, 0, 0);
}
bool isLegalMove(Point start, int dir, Point end) {
	const int dirMid = getNextDir(dir, 4);
	const int dirP = getNextDir(dirMid, 1);
	const int dirN = getNextDir(dirMid, -1);
	return (!(end == performMoveFromDirection(start, dirMid) or end == performMoveFromDirection(start, dirP) or end == performMoveFromDirection(start, dirN)));
}

bool isVisited(int x, int y) {
	return getAlpha(y*WIDTH + x) == 0xff;
}

void addOneLine(Frame frame, Point startPoint) {
	int dir1 = -1, dir2 = -1;
	bool neiTab[3][3] = { { false, false, false },
						{ false, false, false },
						{ false, false, false } };

	for (int i = 1; i <= 9; i++) {
		Point nei = neighbours(startPoint, i);
		if (!(nei == startPoint) and getRed(frame[nei])>0) {
			neiTab[(i - 1) / 3][(i - 1) % 3] = true;
		}
	}

	if (neiTab[0][0] and neiTab[2][2]) dir1 = 4;
	else if (neiTab[0][1] and neiTab[2][1]) dir1 = 5;
	else if (neiTab[0][2] and neiTab[2][0]) dir1 = 2;
	else if (neiTab[1][0] and neiTab[1][2]) dir1 = 3;


	if (dir1 == -1) {
		return; // Corner or st, ignoring
	}
	dir2 = getNextDir(dir1, 4);

	// dir1, dir2 -> possible line directions, we are in the middle(not good)

	Point p1 = traceLineOneSide(frame, startPoint, &dir1, false);
	Point p2 = traceLineOneSide(frame, startPoint, &dir2, false);

	if (distance(startPoint, p1) > distance(startPoint, p2)) {
		dir2 = getNextDir(dir1, 4);
		p2 = traceLineOneSide(frame, p1, &dir2, true); // p1 -> vertex; fincing p2 from p1 with oposite direction
	}
	else {
		dir1 = getNextDir(dir2, 4);
		p1 = traceLineOneSide(frame, p2, &dir1, true);
	}

	//printf("start: (%d, %d), dir: %d/%d : (%d, %d) -> (%d, %d)\n", startPoint.x, startPoint.y, dir1, dir2, p1.x, p1.y, p2.x, p2.y);
	if (isInFrame(p1) and isInFrame(p2) and distance(p1, p2) > 0) {
		addVertexPair(p1.x, p1.y, p2.x, p2.y);
	}
}

Point traceLineOneSide(Frame frame, Point startPoint, int* dir, bool clearPixels) {
	Point currentPoint = performMoveFromDirection(startPoint, *dir);
	Point endPoint = currentPoint;
	Vector mainVector = initVector(startPoint, endPoint);
	Vector tempVector = initVector();
	bool foundNextPoint = true;

	//printf("[%d, %d]:: ", startPoint.x, startPoint.y);

	while (isInFrame(currentPoint) and foundNextPoint) { // main searching loop
		//printf("(%d, %d) :=:", currentPoint.x, currentPoint.y);
		double bestDif = DBL_MAX;
		Point bestPoint;

		foundNextPoint = false;

		for (int i = 1; i <= 9; i++) {
			Point neibour = neighbours(currentPoint, i);
			//printf("(%d, %d) ", neibour.x, neibour.y);
			if (currentPoint == neibour or !isInFrame(neibour) or isEmpty(frame[neibour]) 
				or !isLegalMove(currentPoint, *dir, neibour) 
				or (distance(startPoint, currentPoint) > distance(startPoint, neibour)))
					continue; // the same point or out of frame or empty

			// neibour -> possible next point, need to check
			//printf("* ");
			tempVector = initVector(startPoint, neibour);
			const double thisDif = differenceVector(mainVector, tempVector);
			if (thisDif < bestDif) {
				bestDif = thisDif;
				bestPoint = neibour;
			}
			if (clearPixels) {
				clearPixel(&(frame[neibour]));
			}
		} // end neibours for loop

		// is this move good?
		if (!isVectorClose(mainVector, tempVector, deltaAngle)) { // Not on the line
			//printf(" Not line\n");
			foundNextPoint = false;
			continue;
		}

		foundNextPoint = true; // we have line
		const int tempMove = getMoveFromPoints(currentPoint, bestPoint);
		*dir = getDirectionFromMove(tempMove); // set direction in last move
		mainVector = tempVector;
		currentPoint = bestPoint;
		endPoint = currentPoint;
		//printf(" NEXT line\n");
		
	}
	return endPoint;
}

void addOneLine2(Frame frame, int sx, int sy);
Point traceLineOneSide2(Frame frame, Point startPoint, int dirMain, bool markPixels = false);

void findLanes(Frame frame) {
	int cord = -1;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			cord = y * WIDTH + x;
			if (!isEmpty(frame[cord]))
				addOneLine(frame, { x, y });
		}
	}
	//for (int i = 0; i < WIDTH*HEIGHT; i++) printf("%d ", getAlpha(frame[i]));
}



void addOneLine2(Frame frame, int sx, int sy) {

int cord;
int dir1 = -1, dir2 = -1;
int move1 = -1, move2 = -1;
bool lineFound = false;

bool neiTab[3][3] =	   { {false, false, false },
{ false, false, false },
{ false, false, false }};

for (int b = 0; b < 3; b++) { // Mo¿na mniej
for (int a = 0; a < 3; a++) {
if (a == 1 and b == 1) continue;
cord = 0; // pixel in frame based on a and b offsets
if (sy + b - 1 < 0) cord += 0;
else if (sy + b - 1 >= HEIGHT) cord += (HEIGHT - 1) * WIDTH;
else cord += (sy + b - 1) * WIDTH;
if (sx + a - 1 < 0) cord += 0;
else if (sx + a - 1 >= WIDTH) cord += WIDTH - 1;
else cord += sx + a - 1;
//printf("c: %d ", getRed(frame.content[cord]));
if (getRed(frame.content[cord]) > 0) {
neiTab[a][b] = true;
}
}
} // END direction check


if (neiTab[0][0] and neiTab[2][2]) dir1 = 4;
else if (neiTab[0][1] and neiTab[2][1]) dir1 = 5;
else if (neiTab[0][2] and neiTab[2][0]) dir1 = 2;
else if (neiTab[1][0] and neiTab[1][2]) dir1 = 3;

dir2 = getNextDir(dir1, 4);

if (dir2 == -1 or dir1 == -1) {
	//printf("addOneLine: Ignoring entry: (%d, %d)\n", sx, sy);
	return; // Corner or st, ignoring
}

// Find next corner
Point p1 = traceLineOneSide2(frame, { sx, sy }, dir1, false);
Point p2 = traceLineOneSide2(frame, { sx, sy }, dir2, false);

if (distance({ sx, sy }, p1) > distance({ sx, sy }, p2))
p2 = traceLineOneSide2(frame, p1, dir2, true);
else
p1 = traceLineOneSide2(frame, p2, dir1, true);


//printf("start: (%d, %d), dir: %d/%d : (%d, %d) -> (%d, %d)\n", sx, sy, dir1, dir2, p1.x, p1.y, p2.x, p2.y);
if (isInFrame(p1) and isInFrame(p2) and (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) > minLineLenght) {
	addVertexPair(p1.x, p1.y, p2.x, p2.y);

}
}

Point traceLineOneSide2(Frame frame, Point startPoint, int dirMain, bool markPixels) { //  UWAGA który jest fMIN, fMAX
	enum LAST_DIR {
		DIR_MAIN = 0,
		DIR_MIN,
		DIR_MAX
	};
	LAST_DIR last_dir = DIR_MAIN;
	int status = 0;
	const int maxStatus = 1;
	int dirMin, dirMax;
	int forbiddenDir = -1;
	int listToCheck = 0;
	int checkList;
	Vector fMin = initVector();
	Vector fMax = initVector();
	int px = startPoint.x, py = startPoint.y;

	int tempMove, tempPX, tempPY, tempDir;
	Vector tempVec;
	Point endPoint = { -1, -1 };
	bool foundNextPoint = false;

	if (dirMain % 2 == 0) { // cross
		dirMax = getNextDir(dirMain, 1);
		dirMin = getNextDir(dirMain, -1);
		listToCheck = dirMain * 100 + dirMax * 10 + dirMin;
	}
	else {
		dirMax = getNextDir(dirMain, 2);
		dirMin = getNextDir(dirMain, -2);
		listToCheck = dirMain * 10000 + getNextDir(dirMain, 1) * 1000 + getNextDir(dirMain, -1) * 100 + dirMax * 10 + dirMin;
	}
	//printf("%d\n", listToCheck);
	checkList = listToCheck;
	while (isInFrame({ px, py })) {
		foundNextPoint = false;
		if (markPixels) setVisited(&(frame[py*WIDTH + px]));
		//printf("[%d, %d: %d: %d] ", px, py, markPixels, checkList);

		checkList = listToCheck;
		//printf("(%d, %d) %d :: ", px, py, checkList);
		while (checkList > 0 and !foundNextPoint) {
			tempDir = checkList % 10;
			checkList /= 10;
			//printf(" %d] ", tempDir);
			if (tempDir == forbiddenDir) continue;
			tempMove = getMoveFomDirection(tempDir);
			tempPX = px + (tempMove / 10) - 1;
			tempPY = py + (tempMove % 10) - 1;

			//printf("(%d, %d %d %d %d) ", tempPX, tempPY, isVisited(tempPX, tempPY), getRed(frame[tempPY * WIDTH + tempPX]) > 0, isInFrame({ tempPX, tempPY }));
			if (isInFrame({ tempPX, tempPY }) and getRed(frame[tempPY * WIDTH + tempPX]) > 0 and !isVisited(tempPX, tempPY) and !foundNextPoint) { // line in tempDir direction
				tempVec = initVector(startPoint, { tempPX, tempPY });

				//printf("[[%d, %d %d= %lf %lf %lf %d : %lfl, %lf]] ", tempPX, tempPY, tempDir, fMin, fMax, tempVec, isBetween(fMin, fMax, tempVec), averageVectorDirections(fMin, tempVec), averageVectorDirections(fMax, tempVec));
				if (isBetween(fMin, fMax, tempVec)) { // is this rly line?
					if (status == 0) {
						if (compareDir(dirMain, tempDir) < 0) // dirMin
							fMin = averageVectorDirections(fMin, tempVec);
						else if (compareDir(dirMain, tempDir) > 0) // dirMax
							fMax = averageVectorDirections(fMax, tempVec);
						else {
							if (std::abs(fMin.angle - tempVec.angle) > std::abs(fMax.angle - tempVec.angle)) {
								fMin = averageVectorDirections(fMin, tempVec);
							}
							else {
								fMax = averageVectorDirections(fMax, tempVec);

							}
						}

						endPoint = { tempPX, tempPY };
					}
					else {
						status--;
					}

				} // not rly line, warning++
				else {
					status++;
				}

				foundNextPoint = true; // new point
				forbiddenDir = getNextDir(tempDir, 4);
				px = tempPX;
				py = tempPY;
			}
		} // end checkList while
		  //printf("\n");
		if (status >= maxStatus or !foundNextPoint) return endPoint;
	}
	return endPoint;
}
*/
		// we need to check not-MainDir frst
/*
	// dirMIN
	tempMove = getMoveFomDirection(dirMin);
	tempPX = px + (tempMove / 10) - 1;
	tempPY = py + (tempMove % 10) - 1;

	if (last_dir != DIR_MIN and tempPX >= 0 and tempPX < WIDTH and tempPY >= 0 and tempPY < HEIGHT
	and getRed(frame.content[tempPY * WIDTH + tempPX]) > 0 and !isVisited(tempPX, tempPY)) { // line in dirMin
	//last_dir = DIR_MIN;
	tempVec = initVector(startPoint, { tempPX, tempPY });

	if (isBetween(fMin, fMax, tempVec)) {
	if (status == 0) {
	setVisited(&(frame[tempPY*WIDTH + tempPX]));
	fMin = averageVectorDirections(fMin, tempVec);
	endPoint = { tempPX, tempPY };
	}
	else {
	status--;
	}

	}
	else {
	status++;
	}

	foundNextPoint = true;
	px = tempPX;
	py = tempPY;

	printf("A_");
	}

	// dirMAX
	tempMove = getMoveFomDirection(dirMax);
	tempPX = px + (tempMove / 10) - 1;
	tempPY = py + (tempMove % 10) - 1;
	if (last_dir != DIR_MAX and tempPX >= 0 and tempPX < WIDTH and tempPY >= 0 and tempPY < HEIGHT
	and getRed(frame.content[tempPY * WIDTH + tempPX]) > 0 and !isVisited(tempPX, tempPY)) { // line in dirMAX
	//last_dir = DIR_MAX;
	tempVec = initVector(startPoint, { tempPX, tempPY });

	if (isBetween(fMin, fMax, tempVec)) {
	if (status == 0) {
	setVisited(&(frame[tempPY*WIDTH + tempPX]));
	fMax = averageVectorDirections(fMin, tempVec);
	endPoint = { tempPX, tempPY };
	}
	else {
	status--;
	}

	}
	else {
	status++;
	}

	foundNextPoint = true;
	px = tempPX;
	py = tempPY;

	printf("B_");
	}

	// dirMAIN
	tempMove = getMoveFomDirection(dirMain);
	tempPX = px + (tempMove / 10) - 1;
	tempPY = py + (tempMove % 10) - 1;
	if (tempPX >= 0 and tempPX < WIDTH and tempPY >= 0 and tempPY < HEIGHT
	and getRed(frame.content[tempPY * WIDTH + tempPX]) > 0 and !isVisited(tempPX, tempPY)) { // line in dirMAX
	last_dir = DIR_MAIN;
	tempVec = initVector(startPoint, { tempPX, tempPY });

	if (isBetween(fMin, fMax, tempVec)) {
	if (status == 0) {
	setVisited(&(frame[tempPY*WIDTH + tempPX]));
	endPoint = { tempPX, tempPY };
	}
	else {
	status--;
	}

	}
	else {
	status++;
	}

	foundNextPoint = true;
	px = tempPX;
	py = tempPY;
	printf("C_");
	}
	printf("\n");
	//printf("LOOP: (%d, %d), dirMain: %d, move %d\n", px, py, dirMain, tempMove);

	if (status >= maxStatus or !foundNextPoint) return endPoint;
	} // end main WHILE
} */