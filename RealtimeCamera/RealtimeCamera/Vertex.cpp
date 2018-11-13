#include "stdafx.h"
#include "Vertex.h"

int VertexTabID = 0;
Vertex* VertexTab;

void initVertexTab() {
	VertexTab = new Vertex[VertexTabLength];

	for (int i = 0; i < VertexTabLength; i++) {
		VertexTab[i].nodes = new int[defaultNodesSize];
		VertexTab[i].nodesSize = VertexTabLength;
		VertexTab[i].nodeID = 0;
	}
}

void deinitVertexTab() {
	delete[] VertexTab;
}

void printVertex() {
	printf("VERTEX:\n");
	for (int i = 0; i < VertexTabID; i++) {
		printf("[%d]: (%d, %d) | %d :", i, VertexTab[i].x, VertexTab[i].y, VertexTab[i].nodeID);
		for (int q = 0; q < VertexTab[i].nodeID; q++)
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
	if (v->nodeID == v->nodesSize) return false;
	for (int i = 0; i < v->nodeID; i++) {
		if (v->nodes[i] == dest) return true;
	}
	v->nodes[v->nodeID] = dest;
	v->nodeID++;
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

void addVertexPair(int x1, int y1, int x2, int y2) {
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

void drawLine(Frame* frame, Vertex* v1, Vertex* v2) {
	double a = (double)(v1->y - v2->y) / (v1->x - v2->x);
	double b = (double)(v1->y+0.5) - a * (v1->x+0.5);

	if (std::abs(v1->x - v2->x) > std::abs(v1->y - v2->y)) { // iter XX
		const int Xmax = std::max(v1->x, v2->x);
		const int Xmin = std::min(v1->x, v2->x);
		int itX = Xmin;
		int py = -1;
		while (itX <= Xmax) {
			py = (a * itX + b);
			setAllColors(&(frame->content[py * WIDTH + itX]));
			itX++;
		}
	} else { // iter YY
		const int Ymax = std::max(v1->y, v2->y);
		const int Ymin = std::min(v1->y, v2->y);
		int itY = Ymin;
		int px = -1;
		while (itY <= Ymax) {
			px = ((itY - b) / a);
			setAllColors(&(frame->content[itY * WIDTH + px]));
			itY++;
		}
	}

	int cord1 = v1->y * WIDTH + v1->x;
	int cord2 = v2->y * WIDTH + v2->x;
	setAllColors(&(frame->content[cord1]), 0xff, 0x00, 0x00);
	setAllColors(&(frame->content[cord2]), 0x00, 0x00, 0xff);
}

void generateFrameForVertex(Frame* frame, bool clearTab) {
	clearFrame(frame);
	for (int i = 0; i < VertexTabID; i++) {

		int nodeID = VertexTab[i].nodeID;
		for (int q = 0; q < nodeID; q++) {
			if (i < VertexTab[i].nodes[q]) {
				drawLine(frame, &VertexTab[i], &VertexTab[VertexTab[i].nodes[q]]);
			}
		}

		if (clearTab) {
			VertexTab[i].x = -1;
			VertexTab[i].y = -1;
			VertexTab[i].nodeID = 0;
		}
	}
	if (clearTab) VertexTabID = 0;
}