#include "stdafx.h"
#include "VertexPostprocesing.h"

bool isInLineProxy(int v1Start, int v1End, int v2Start, int v2End) {
	return false;
}

void mergeVertex(int midPoz, int v1Poz, int v2Poz) {
	Vertex v1 = VertexTab[v1Poz];
	for (int i = 0; i < v1.nodeID; i++) {
		if (v1.nodes[i] == midPoz) {
			v1.nodes[i] = v2Poz;
			break;
		}
	}

	v1 = VertexTab[v2Poz];
	for (int i = 0; i < v1.nodeID; i++) {
		if (v1.nodes[i] == midPoz) {
			v1.nodes[i] = v1Poz;
			break;
		}
	}
	VertexTab[midPoz].x = 0;
	VertexTab[midPoz].y = 0;
	VertexTab[midPoz].nodeID = 0;
}

void handleInLineConnections(int vMidPoz) {
	Vertex vMid = VertexTab[vMidPoz];
	int v1Poz = vMid.nodes[0];
	int v2Poz = vMid.nodes[1];
	const Vertex v1 = VertexTab[v1Poz];
	const Vertex v2 = VertexTab[v2Poz];
	Line line({ v1.x, v1.y }, { v2.x, v2.y });
	Line l1({ v1.x, v1.y }, { vMid.x, vMid.y });
	Line l2({ vMid.x, vMid.y }, { v2.x, v2.y });

	const int dist = std::max(line.distanceFrom({ v1.x, v1.y }), line.distanceFrom({ v2.x, v2.y }));
	const double angle = angleCalculate(l1, l2);
	if (dist < MERGE_POINT_LINE_DISTANCE and angle < MERGE_LINE_ANGLE) {
		mergeVertex(vMidPoz, v1Poz, v2Poz);
	}
}

void vertexPostprocesing() {
	int n = 0;
	for (int i = 0; i < VertexTabID; i++) {
		if (VertexTab[i].nodeID == 2) {
			handleInLineConnections(i);
			n++;
		}
	}
	printf("n: %d\n", n);
}
