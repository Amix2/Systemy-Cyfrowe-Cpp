#include "stdafx.h"
#include "LineFinder.h"

const int minLineLenght = HEIGHT/10;
const int deltaAngle = 5 / (2 * PI);

Point traceLineOneSide(Frame frame, Point startPoint, int* dir, bool clearPixels = false);
void addOneLine(Frame frame, Point startPoint);

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

	printf("start: (%d, %d), dir: %d/%d : (%d, %d) -> (%d, %d)\n", startPoint.x, startPoint.y, dir1, dir2, p1.x, p1.y, p2.x, p2.y);
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

void findLanes(Frame frame) {
	int cord = -1;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			cord = y * WIDTH + x;
			if (getRed(frame[cord]) > 0)
				addOneLine(frame, { x, y });
		}
	}
	//for (int i = 0; i < WIDTH*HEIGHT; i++) printf("%d ", getAlpha(frame[i]));
}

/*

void addOneLine(Frame frame, int sx, int sy) {

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
/*
printf("\nNeiTab (%d, %d)\n", sx, sy);
for (int a = 0; a < 3; a++) {
for (int b = 0; b < 3; b++) {
printf("%d ", neiTab[a][b]);
}
printf("\n");
}
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
Point p1 = traceLineOneSide(frame, { sx, sy }, dir1, false);
Point p2 = traceLineOneSide(frame, { sx, sy }, dir2, false);

if (distance({ sx, sy }, p1) > distance({ sx, sy }, p2))
p2 = traceLineOneSide(frame, p1, dir2, true);
else
p1 = traceLineOneSide(frame, p2, dir1, true);


//printf("start: (%d, %d), dir: %d/%d : (%d, %d) -> (%d, %d)\n", sx, sy, dir1, dir2, p1.x, p1.y, p2.x, p2.y);
if (isInFrame(p1) and isInFrame(p2) and (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) > minLineLenght) {
	addVertexPair(p1.x, p1.y, p2.x, p2.y);

}
}

Point traceLineOneSide(Frame frame, Point startPoint, int dirMain, bool markPixels) { //  UWAGA który jest fMIN, fMAX
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
		printf("(%d, %d) %d :: ", px, py, checkList);
		while (checkList > 0 and !foundNextPoint) {
			tempDir = checkList % 10;
			checkList /= 10;
			//printf(" %d] ", tempDir);
			if (tempDir == forbiddenDir) continue;
			tempMove = getMoveFomDirection(tempDir);
			tempPX = px + (tempMove / 10) - 1;
			tempPY = py + (tempMove % 10) - 1;

			printf("(%d, %d %d %d %d) ", tempPX, tempPY, isVisited(tempPX, tempPY), getRed(frame[tempPY * WIDTH + tempPX]) > 0, isInFrame({ tempPX, tempPY }));
			if (isInFrame({ tempPX, tempPY }) and getRed(frame[tempPY * WIDTH + tempPX]) > 0 and !isVisited(tempPX, tempPY) and !foundNextPoint) { // line in tempDir direction
				tempVec = initVector(startPoint, { tempPX, tempPY });

				printf("[[%d, %d %d= %lf %lf %lf %d : %lfl, %lf]] ", tempPX, tempPY, tempDir, fMin, fMax, tempVec, isBetween(fMin, fMax, tempVec), averageVectorDirections(fMin, tempVec), averageVectorDirections(fMax, tempVec));
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
	/*	// we need to check not-MainDir frst

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
	*/