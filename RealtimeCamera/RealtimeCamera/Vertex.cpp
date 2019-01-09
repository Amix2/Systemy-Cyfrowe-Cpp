#include "stdafx.h"
#include "Vertex.h"

int VertexTabID = 0;
Vertex* VertexTab;

void initVertexTab() {
	VertexTab = new Vertex[VertexTabLength];

	for (int i = 0; i < VertexTabLength; i++) {
		VertexTab[i].nodes = new int[defaultNodesSize];
		VertexTab[i].nodesSize = VertexTabLength;
		VertexTab[i].nodeFirstFreeID = 0;
	}
}

void deinitVertexTab() {
	delete[] VertexTab;
}

void clearVertex() {
	for (int i = 0; i < VertexTabID; i++) {
		VertexTab[i].x = -1;
		VertexTab[i].y = -1;
		VertexTab[i].nodeFirstFreeID = 0;
	}
	VertexTabID = 0;
}

void printVertex() {
	printf("VERTEX:\n");
	for (int i = 0; i < VertexTabID; i++) {
		printf("[%d]: (%d, %d) | %d :", i, VertexTab[i].x, VertexTab[i].y, VertexTab[i].nodeFirstFreeID);
		for (int q = 0; q < VertexTab[i].nodeFirstFreeID; q++)
			printf("%d ", VertexTab[i].nodes[q]);
		printf("\n");
	}
	printf("======\n");
}

void printVertexSize() {
	printf("Vertex count: %d\n", VertexTabID);
}

bool equals(Vertex v1, Vertex v2) {
	return v1.x == v2.x and v1.y == v2.y;
}

bool equals(Vertex v1, int x2, int y2) {
	return v1.x == x2 and v1.y == y2;
}

bool equals(int x1, int y1, int x2, int y2) {
	return x1 == x2 and y1 == y2;
}

bool addNodeToVertex(Vertex* v, int dest) {
	if (&(VertexTab[dest]) == v) return true;
	if (v->nodeFirstFreeID == v->nodesSize) return false;
	for (int i = 0; i < v->nodeFirstFreeID; i++) {
		if (v->nodes[i] == dest) return true;
	}
	v->nodes[v->nodeFirstFreeID] = dest;
	v->nodeFirstFreeID++;
	return true;
}

Vertex* findVertex(int x, int y) {
	for (int i = 0; i < VertexTabLength; i++) {
		if (equals(VertexTab[i], x, y)) {
			return &VertexTab[i];
		}
	}
	return NULL;
}

int findPlaceForNewPoint(const Point& point) {
	int vertPoz = -1;
	double dist = VertexProximity;
	for (int i = 0; i < VertexTabID; i++) {
		const double newDist = distance(point, { VertexTab[i].x, VertexTab[i].y });
		if (dist >  newDist) { // we have better point to merge
			//printf("dist: %lf\n", newDist);
			vertPoz = i;
			dist = newDist;
		}
	}
	if (vertPoz == -1) {
		//printf("NEW: %d, from %d, %d\n", VertexTabID, point.x, point.y);
		VertexTabID++;
		return VertexTabID - 1;
	}
	VertexTab[vertPoz].x = (VertexTab[vertPoz].x + point.x) / 2;
	VertexTab[vertPoz].y = (VertexTab[vertPoz].y + point.y) / 2;
	//printf("OLD: %d, from %d, %d\n", vertPoz, point.x, point.y);
	return vertPoz;
}

void addVertexPair(int x1, int y1, int x2, int y2) {
	const int v1Poz = findPlaceForNewPoint({ x1, y1 });
	Vertex* v1 = &(VertexTab[v1Poz]);
	v1->x = x1;
	v1->y = y1;

	const int v2Poz = findPlaceForNewPoint({ x2, y2 });
	Vertex* v2 = &(VertexTab[v2Poz]);
	v2->x = x2;
	v2->y = y2;

	//printf("addVertexPair: %d,%d -> %d, %d, POZ: %d -> %d\n", x1, y1, x2, y2, v1Poz, v2Poz);

	if (!addNodeToVertex(v1, v2Poz)) std::cout << "ERROR: can not add Node To Vertex" << std::endl;

	if (!addNodeToVertex(v2, v1Poz)) std::cout << "ERROR: can not add Node To Vertex" << std::endl;
}

void addVertexPair(Point p1, Point p2) {
	addVertexPair(p1.x, p1.y, p2.x, p2.y);
}

void drawLine(Frame* frame, Vertex* v1, Vertex* v2, uint8_t red, uint8_t blue, uint8_t green) {
	const double con1 = v1->x - v2->x;
	const double con2 = v1->y - v2->y;
	const double A = -con2;
	const double B = con1;
	const double C = v1->x*con2 - v1->y*con1;

	const double a = (double)(v1->y - v2->y) / (v1->x - v2->x);
	const double b = (double)(v1->y+0.5) - a * (v1->x+0.5);

	if (std::abs(v1->x - v2->x) > std::abs(v1->y - v2->y)) { // iter XX
		const int Xmax = std::max(v1->x, v2->x);
		const int Xmin = std::min(v1->x, v2->x);
		int itX = Xmin;
		int py = -1;
		while (itX <= Xmax) {
			py = round((A*itX + C) / (-B));
			setAllColors(&(frame->content[py * WIDTH + itX]), red, green, blue);
			itX++;
		}
	} else { // iter YY
		const int Ymax = std::max(v1->y, v2->y);
		const int Ymin = std::min(v1->y, v2->y);
		int itY = Ymin;
		int px = -1;
		while (itY <= Ymax) {
			px = round((B*itY + C) / (-A));
			setAllColors(&(frame->content[itY * WIDTH + px]), red, green, blue);
			itY++;
		}
	}
}

void generateFrameForVertex(Frame* frame, bool clearTab) {
	clearFrame(frame);
	for (int i = 0; i < VertexTabID; i++) {

		int nodeFirstFreeID = VertexTab[i].nodeFirstFreeID;
		for (int q = 0; q < nodeFirstFreeID; q++) {
			if (i < VertexTab[i].nodes[q]) {
				drawLine(frame, &VertexTab[i], &VertexTab[VertexTab[i].nodes[q]]);
			}
		}

		if (clearTab) {
			VertexTab[i].x = -1;
			VertexTab[i].y = -1;
			VertexTab[i].nodeFirstFreeID = 0;
		}
	}
	if (clearTab) VertexTabID = 0;
}

void putVertexOnFrame(Frame* frame, bool clearTab) {
	for (int i = 0; i < VertexTabID; i++) {
		int nodeFirstFreeID = VertexTab[i].nodeFirstFreeID;
		for (int q = 0; q < nodeFirstFreeID; q++) {
			if (i < VertexTab[i].nodes[q]) {
				drawLine(frame, &VertexTab[i], &VertexTab[VertexTab[i].nodes[q]]);
			}
		}

		if (clearTab) {
			VertexTab[i].x = -1;
			VertexTab[i].y = -1;
			VertexTab[i].nodeFirstFreeID = 0;
		}
	}
	if (clearTab) VertexTabID = 0;
}

void generateFancyGroupFrameForVertex(Frame* frame, bool clearTab) {
	int *colorTab = new int[VertexTabID];
	for (int i = 0; i < VertexTabID; i++) colorTab[i] = -1;
	clearFrame(frame);
	for (int i = 0; i < VertexTabID; i++) {
		int color;
		if (colorTab[i] != -1) color = colorTab[i];
		else {
			const int red = (rand() % 10) * 25;
			const int green = (rand() % 10) * 25;
			const int blue = (rand() % 10) * 25;
			setAllColors(&color, red, green, blue);
			colorTab[i] = color;
		}
		const int nodeFirstFreeID = VertexTab[i].nodeFirstFreeID;
		for (int q = 0; q < nodeFirstFreeID; q++) { // setting color to every conected line 
			if (i < VertexTab[i].nodes[q]) {
				colorTab[VertexTab[i].nodes[q]] = color;
			}
		}
	}
	for (int i = 0; i < VertexTabID; i++) {
		int nodeFirstFreeID = VertexTab[i].nodeFirstFreeID;
		for (int q = 0; q < nodeFirstFreeID; q++) {
			if (i < VertexTab[i].nodes[q]) {
				drawLine(frame, &VertexTab[i], &VertexTab[VertexTab[i].nodes[q]], getRed(colorTab[i]), getBlue(colorTab[i]), getGreen(colorTab[i]));
			}
		}
		if (clearTab) {
			VertexTab[i].x = -1;
			VertexTab[i].y = -1;
			VertexTab[i].nodeFirstFreeID = 0;
		}
	}
	if (clearTab) VertexTabID = 0;
	free(colorTab);
}

void generateFancyLineFrameForVertex(Frame* frame, bool clearTab) {
	clearFrame(frame);
	for (int i = 0; i < VertexTabID; i++) {

		const int nodeFirstFreeID = VertexTab[i].nodeFirstFreeID;
		for (int q = 0; q < nodeFirstFreeID; q++) {
			int color;
			const int red = ((rand() % 5)+5) * 25;
			const int green = ((rand() % 5) + 5) * 25;
			const int blue = ((rand() % 5) + 5) * 25;
			setAllColors(&color, red, green, blue);
			if (i < VertexTab[i].nodes[q]) {
				drawLine(frame, &VertexTab[i], &VertexTab[VertexTab[i].nodes[q]], getRed(color), getBlue(color), getGreen(color));
			}
		}
		setRed(&(frame->content[VertexTab[i].y * WIDTH + VertexTab[i].x]), 0xFF);
		if (clearTab) {
			VertexTab[i].x = -1;
			VertexTab[i].y = -1;
			VertexTab[i].nodeFirstFreeID = 0;
		}
	}
	if (clearTab) VertexTabID = 0;
}

void generateBordersFrameForVertex(Frame* frame, bool clearTab) {
	clearFrame(frame);
	for (int i = 0; i < VertexTabID; i++) {
		setRed(&(frame->content[VertexTab[i].y * WIDTH + VertexTab[i].x]), 0xFF);
		if (clearTab) {
			VertexTab[i].x = -1;
			VertexTab[i].y = -1;
			VertexTab[i].nodeFirstFreeID = 0;
		}
	}
	if (clearTab) VertexTabID = 0;
}

/*
void addVertexPair2(int x1, int y1, int x2, int y2) {
Vertex *v1 = NULL, *v2 = NULL;
int id1 = -1, id2 = -1;
int dist1 = VertexProximity, dist2 = VertexProximity;

for (int i = 0; (v1 == NULL or v2 == NULL) and i < VertexTabID; i++) {
if (v1 == NULL and equals(VertexTab[i], x1, y1)) {
v1 =  &VertexTab[i];
id1 = i;
}
else if (distance({ x1, y1 }, { VertexTab[i].x, VertexTab[i].y }) < dist1) {
dist1 = distance({ x1, y1 }, { VertexTab[i].x, VertexTab[i].y });
id1 = i;
}

if (v2 == NULL and equals(VertexTab[i], x2, y2)) {
v2 = &VertexTab[i];
id2 = i;
}
else if (distance({ x2, y2 }, { VertexTab[i].x, VertexTab[i].y }) < dist1) {
dist1 = distance({ x2, y2 }, { VertexTab[i].x, VertexTab[i].y });
id2 = i;
}
}
if(id1 == -1) {
v1 = &VertexTab[VertexTabID];
id1 = VertexTabID;
VertexTabID++;
}
else if (id1 >= 0 and v1 == NULL) {
v1 = &VertexTab[id1];
}
if (id2 == -1) {
v2 = &VertexTab[VertexTabID];
id2 = VertexTabID;
VertexTabID++;
}
else if (id2 >= 0 and v2 == NULL) {
v2 = &VertexTab[id2];
}

v1->x = x1;
v1->y = y1;
v2->x = x2;
v2->y = y2;

if (!addNodeToVertex(v1, id2)) std::cout << "ERROR: can not add Node To Vertex" << std::endl;

if (!addNodeToVertex(v2, id1)) std::cout << "ERROR: can not add Node To Vertex" << std::endl;
}
*/