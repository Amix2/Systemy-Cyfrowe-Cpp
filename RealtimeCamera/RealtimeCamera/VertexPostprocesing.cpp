#include "stdafx.h"
#include "VertexPostprocesing.h"

bool isInLineProxy(int v1Start, int v1End, int v2Start, int v2End) {
	return false;
}

void mergeVertex(int midPoz, int v1Poz, int v2Poz) {
	Vertex v1 = VertexTab[v1Poz];
	for (int i = 0; i < v1.nodeFirstFreeID; i++) {
		if (v1.nodes[i] == midPoz) {
			v1.nodes[i] = v2Poz;
			break;
		}
	}

	v1 = VertexTab[v2Poz];
	for (int i = 0; i < v1.nodeFirstFreeID; i++) {
		if (v1.nodes[i] == midPoz) {
			v1.nodes[i] = v1Poz;
			break;
		}
	}
	VertexTab[midPoz].x = 0;
	VertexTab[midPoz].y = 0;
	VertexTab[midPoz].nodeFirstFreeID = 0;
}

void handleInLineConnections(int vMidPoz) {
	Vertex vMid = VertexTab[vMidPoz];
	int v1Poz = vMid.nodes[0];
	int v2Poz = vMid.nodes[1];
	const Vertex v1 = VertexTab[v1Poz];
	const Vertex v2 = VertexTab[v2Poz];
	Line line({ v1.x, v1.y }, { v2.x, v2.y });

	const double angle1 = angleCalculate(Point(v1.x, v1.y), { vMid.x, vMid.y });
	const double angle2 = angleCalculate(Point(vMid.x, vMid.y), { v2.x, v2.y });
	const double angleDif = angleDifference(angle1, angle2);
	const int dist = line.distanceFrom({ vMid.x, vMid.y });

	if (dist < MERGE_POINT_LINE_DISTANCE and angleDif < MERGE_LINE_ANGLE) {
		mergeVertex(vMidPoz, v1Poz, v2Poz);
	}
}

void vertexPostprocesing() {
	int n = 0;
	for (int i = 0; i < VertexTabID; i++) {
		if (VertexTab[i].nodeFirstFreeID == 2) {
			handleInLineConnections(i);
			n++;
		}
	}
	printf("vertexPostprocesing:: n: %d\n", n);
}
