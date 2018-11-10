#include "stdafx.h"
#include "CameraHandler.h"

struct SimpleCapParams myCapture;
int selectedDevice = 0;

int initCamera(int device){
	selectedDevice = device;
	int devices = setupESCAPI();
	myCapture.mWidth = WIDTH;
	myCapture.mHeight = HEIGHT;
	myCapture.mTargetBuf = new int[WIDTH * HEIGHT];
	std::cout<<"Devices: "<< devices << std::endl;
	if (devices == 0)
	{
		printf("ESCAPI initialization failure or no devices found.\n");
		return -1;
	}
	if (initCapture(selectedDevice, &(myCapture)) == 0)
	{
		printf("Capture failed - device may already be in use.\n");
		return -2;
	}
	return 0;
}

void deinitCamera() {
	deinitCapture(selectedDevice);
}

void captureFrame(Frame *frame) {
	doCapture(selectedDevice);
	while (isCaptureDone(selectedDevice) == 0);
	frame->content = myCapture.mTargetBuf;
}