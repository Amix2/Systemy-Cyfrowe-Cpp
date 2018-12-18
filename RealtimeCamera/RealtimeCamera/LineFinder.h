#pragma once
#include "stdafx.h"
#include "Vertex.h"
#include "ImageProcessing.h"
#include "MathFunctions.h"
#include "Stripe.h"


void findLanes(Frame frame);
int isGodStartingPoint(Frame frame, Point point);
Point traceLine(Frame frame, Point startPoint, Point secPoint, Stripe* stripe, bool markVisited = false);
Point findNextBestStartingPoint(Frame frame, const Point point, int dir);
Point findEndOfContinueLine(Frame frame, Point point);