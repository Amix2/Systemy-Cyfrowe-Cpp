#pragma once
#include "stdafx.h"
#include "ImageProcessing.h"
#include "MathFunctions.h"
#include <algorithm>


typedef struct Vertex {
	int x;
	int y;
	int* nodes;
	int nodeID = 0;
	int nodesSize = defaultNodesSize;
} Vertex;

extern int VertexTabID;
extern Vertex* VertexTab;

void initVertexTab();
void deinitVertexTab();
void printVertex();
bool equals(Vertex v1, Vertex v2);
bool equals(Vertex v1, int x2, int y2);
bool equals(int x1, int y1, int x2, int y2);
bool addNodeToVertex(Vertex* v, int dest);
Vertex* findVertex(int x, int y);
void addVertexPair(int x1, int y1, int x2, int y2);
void drawLine(Frame* frame, Vertex* v1, Vertex* v2, uint8_t red = 0xff, uint8_t blue = 0xff, uint8_t green = 0xff);
void generateFrameForVertex(Frame* frame, bool clearTab = true);
void putVertexOnFrame(Frame* frame, bool clearTab = true);
void generateFancyLineFrameForVertex(Frame* frame, bool clearTab = true);
void generateFancyGroupFrameForVertex(Frame* frame, bool clearTab = true);
void generateBordersFrameForVertex(Frame* frame, bool clearTab = true);

void printVertexSize();
