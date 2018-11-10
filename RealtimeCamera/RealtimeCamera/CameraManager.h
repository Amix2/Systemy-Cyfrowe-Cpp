#pragma once

#include "escapi.h"
#include "stdafx.h"
#include "Frame.h"

class CameraManager {
	struct SimpleCapParams capture;
public:
	CameraManager(int, int);
	~CameraManager();
	FrameRGB* getRGBFrame();
};
