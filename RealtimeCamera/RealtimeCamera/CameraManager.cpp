#include "stdafx.h"
#include "CameraManager.h"

CameraManager::CameraManager(int width, int height) {
	LOG("CameraManager Construcor")
	int devices = setupESCAPI();

	if (devices == 0)
	{
		printf("ESCAPI initialization failure or no devices found.\n");
	}

	this->capture.mWidth = width;
	this->capture.mHeight = height;
	this->capture.mTargetBuf = new int[width * height];
	if (initCapture(0, &(this->capture)) == 0)
	{
		printf("Capture failed - device may already be in use.\n");
	}
}

CameraManager::~CameraManager() {
	deinitCapture(0);
}

FrameRGB* CameraManager::getRGBFrame() {
	LOG("CameraManager get RGB Frame")
	doCapture(0);
	return (new FrameRGB(this->capture));
}