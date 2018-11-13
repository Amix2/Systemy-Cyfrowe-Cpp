#pragma once
#include "stdafx.h"
#include "ImageProcessing.h"
#include "MathFunctions.h"
#include <algorithm>

const int defaultNodesSize = 100;
const int VertexTabLength = 10000;
const int VertexProximity = HEIGHT/20;

typedef struct Vertex {
	int x;
	int y;
	int* nodes;
	int nodeID = 1;
	int nodesSize = defaultNodesSize;
} Vertex;

void initVertexTab();
void deinitVertexTab();
void printVertex();
bool equals(Vertex v1, Vertex v2);
bool equals(Vertex v1, int x2, int y2);
bool equals(int x1, int y1, int x2, int y2);
bool addNodeToVertex(Vertex* v, int dest);
Vertex* findVertex(int x, int y);
void addVertexPair(int x1, int y1, int x2, int y2);
void drawLine(Frame* frame, Vertex* v1, Vertex* v2);
void generateFrameForVertex(Frame* frame, bool clearTab = true);

void printVertexSize();
