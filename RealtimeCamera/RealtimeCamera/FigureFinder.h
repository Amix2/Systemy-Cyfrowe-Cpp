#pragma once
#include "stdafx.h"
#include "Polygon.h"
#include "Parallelogram.h"

void findSquaresInVertexIntoArray(PolygonFigure polygonArray[], int polyArraySize);
void findSquaresInVertexIntoFrame(Frame* frame);
void findSquaresInVertex();
bool isSquare(PolygonFigure polygon, Parallelogram square);