#include "stdafx.h"
#include "FigureFinder.h"
#include "Polygon.h"
#include "Vertex.h"

bool SearchVertexIntoPolygonDFS(int startPointID, int currentPointID, int maxNumOfSteps, PolygonFigure* polygon, int nextPointQueue[], int neiIDs[], int queueID);

void findSquaresInVertexIntoArray(PolygonFigure polygonArray[], int polyArraySize) {
	int polyArrayID = 0;
	bool goodPoly = false;
	int maxNumOfSteps = 15;

	int nextPointQueue[20];
	for (int i = 0; i < 20; i++) nextPointQueue[i] = -1;

	int neiIDs[20];
	for (int i = 0; i < 20; i++) neiIDs[i] = -1;

	for (int pn = 0; pn < VertexTabID; pn++) {
		for (int nei = 0; nei < VertexTab[pn].nodeFirstFreeID; nei++) {
			if (VertexTab[pn].nodes[nei] < pn) continue; // ignoring points wiej smaller index
			PolygonFigure polygon(20);
			if (nextPointQueue[0] != -1) {	//comeback loop
				//printf("goodPoly comebackLoop pn: %d,  nei: %d,  to: %d\n", pn, nei, nextPointQueue[0]);
				goodPoly = SearchVertexIntoPolygonDFS(pn, nextPointQueue[0], maxNumOfSteps - 1, &polygon, nextPointQueue, neiIDs, 0);
			}
			else {
				//printf("goodPoly normal nei:: pn: %d,  nei: %d\n", pn, nei);
				goodPoly = SearchVertexIntoPolygonDFS(pn, VertexTab[pn].nodes[nei], maxNumOfSteps - 1, &polygon, nextPointQueue, neiIDs, 0);
			}
			if (nextPointQueue[0] != -1) {
				nei--;
			}
			//for (int i = 0; i < 10; i++) printf("%d ", nextPointQueue[i]);
			//printf("\n");
			//for (int i = 0; i < 10; i++) printf("%d ", neiIDs[i]);
			//printf("\n");
			//printf("=====\tNEI: %d\n", nei);
			polygon.print();
			if (goodPoly) {
				Parallelogram square(polygon);
				square.print();
				if (isSquare(polygon, square)) {
					polygonArray[polyArrayID] = PolygonFigure(polygon);
					polyArrayID++;
					if (polyArrayID == polyArraySize) return;
				}
			}
		}
	}
}

void findSquaresInVertexIntoFrame(Frame * frame) {
	int polyArrayID = 0;
	bool goodPoly = false;
	int maxNumOfSteps = 15;

	int nextPointQueue[20];
	for (int i = 0; i < 20; i++) nextPointQueue[i] = -1;

	int neiIDs[20];
	for (int i = 0; i < 20; i++) neiIDs[i] = -1;

	for (int pn = 0; pn < VertexTabID; pn++) {
		for (int nei = 0; nei < VertexTab[pn].nodeFirstFreeID; nei++) {
			//printf("=====\tpn: %d NEI: %d\n", pn, nei);
			if (VertexTab[pn].nodes[nei] < pn) continue; // ignoring points wiej smaller index
			PolygonFigure polygon(20);
			if (nextPointQueue[0] != -1) {	//comeback loop
											//printf("goodPoly comebackLoop pn: %d,  nei: %d,  to: %d\n", pn, nei, nextPointQueue[0]);
				goodPoly = SearchVertexIntoPolygonDFS(pn, nextPointQueue[0], maxNumOfSteps - 1, &polygon, nextPointQueue, neiIDs, 0);
			}
			else {
				//printf("goodPoly normal nei:: pn: %d,  nei: %d\n", pn, nei);
				goodPoly = SearchVertexIntoPolygonDFS(pn, VertexTab[pn].nodes[nei], maxNumOfSteps - 1, &polygon, nextPointQueue, neiIDs, 0);
			}
			if (nextPointQueue[0] != -1) {
				nei--;
			}
			//for (int i = 0; i < 10; i++) printf("%d ", nextPointQueue[i]);
			//printf("\n");
			//for (int i = 0; i < 10; i++) printf("%d ", neiIDs[i]);
			//printf("\n");
			if (goodPoly) {
				//printf("xxx\n");
				//polygon.print();
				//polygon.print();
				Parallelogram square(polygon);
				//square.print();
				if (isSquare(polygon, square)) {
					square.draw(frame);
				}
			}
		}
	}
}

void findSquaresInVertex() {
	PolygonFigure polygonArray[20];
	findSquaresInVertexIntoArray(polygonArray, 20);
}

bool isSquare(PolygonFigure polygon, Parallelogram square) {
	const double polyArea = polygon.volume();
	const double squareArea = square.volume();
	//printf("isSquare: polyArea: %0.2f, squareArea: %0.2f\n", polyArea, squareArea);

	return (1 - POLYGON_SQUARE_APPROX)*squareArea <= polyArea and (1 + POLYGON_SQUARE_APPROX)*squareArea >= polyArea;
}

bool SearchVertexIntoPolygonDFS(int startPointID, int currentPointID, int maxNumOfSteps, PolygonFigure* polygon, int nextPointQueue[], int neiIDs[], int queueID) { // polygon

	//printf("SearchDFS\tstart: %d\tcur: %d\t, nextP: %d\n", startPointID, currentPointID, nextPointQueue[queueID+1]);
	if (queueID == 0) {
		VertexTab[currentPointID].maxVisitedByID = startPointID;
		// usuwanie koñczenia 2 razy
	}
	for (int i = 0; i < queueID; i++) {	// pêtla nie do pierwszego elementu = abort
		if (nextPointQueue[i] == currentPointID) {
			nextPointQueue[queueID] = -1;
			neiIDs[queueID] = -1;
			//printf("small loop cur: %d\n", currentPointID);
			return false;
		}
	}

	bool isGood = false;

	int searchID = 0;	//poszukiwanie kolejnych s¹siadów zacznie sie od tego indeksu
	if (neiIDs[queueID] != -1) searchID = neiIDs[queueID];	// comeback

	//printf("searchID: %d\n", searchID);
	if (currentPointID == startPointID) { // mamy pêtle
		//printf("Loop: ");
		if (queueID >= 3) {
			//printf("good\n");
			polygon->addPoint(Point(VertexTab[currentPointID].x, VertexTab[currentPointID].y));
			return true;
		}
		else {
			return false;
			//printf("bad\n");
		}
	}

	if (maxNumOfSteps == 0) return false;

	if (nextPointQueue[queueID+1] != -1) { // comeback points
		isGood = SearchVertexIntoPolygonDFS(startPointID, nextPointQueue[queueID+1], maxNumOfSteps - 1, polygon, nextPointQueue, neiIDs, queueID +1);
	}

	nextPointQueue[queueID] = currentPointID;
	//rekurencja
	for (; !isGood and searchID < VertexTab[currentPointID].nodeFirstFreeID; searchID++) {
		//printf("LOOP\n[%d -> %d]:: searchID: %d\n", startPointID, currentPointID, searchID);
		//printf("\tstart: %d\tcur: %d\t, nextP: %d\n", startPointID, currentPointID, nextPointQueue[queueID + 1]);
		//for (int i = 0; i < 10; i++) printf("%d ", nextPointQueue[i]);
		//printf("\n");
		//for (int i = 0; i < 10; i++) printf("%d ", neiIDs[i]);
		//printf("\n");
		if (VertexTab[currentPointID].nodes[searchID] < startPointID) continue; // pomiñ jak ID jest mniejsze, mo¿na iœæ tylko po coraz wiêkszych indeksach
		//if (VertexTab[currentPointID].nodes[searchID] == startPointID and VertexTab[currentPointID].maxVisitedByID == startPointID) continue;

		neiIDs[queueID] = searchID+1;
		isGood = SearchVertexIntoPolygonDFS(startPointID, VertexTab[currentPointID].nodes[searchID], maxNumOfSteps-1, polygon, nextPointQueue, neiIDs, queueID +1);
		//printf("[%d] LOOP\tsearchID END: %d\n", currentPointID, searchID);
	}
	//printf("After LOOP: %d\n", isGood);
	//for (int i = 0; i < 10; i++) printf("%d ", nextPointQueue[i]);
	//printf("\n");
	//for (int i = 0; i < 10; i++) printf("%d ", neiIDs[i]);
	//printf("\n");

	if (isGood) { // mamy pêtle i jesteœmy w figurze
		polygon->addPoint(Point(VertexTab[currentPointID].x, VertexTab[currentPointID].y));
	}
	else {
		nextPointQueue[queueID] = -1;
		neiIDs[queueID] = -1;
	}
	return isGood;
}
