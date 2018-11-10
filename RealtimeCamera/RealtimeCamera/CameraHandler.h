#pragma once
#include "stdafx.h"
#include "escapi.h"


int initCamera(int device = 0);
void deinitCamera();
void captureFrame(Frame *frame);