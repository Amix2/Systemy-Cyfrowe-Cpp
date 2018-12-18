#pragma once
#include "stdafx.h"

struct PointQueue {
	int size;
	Point* array;
	int takeId = 0; // pierwsze z elementem
	int addId = 0; // pierwsze wolne

	PointQueue(int s);
	~PointQueue();

	Point take();
	bool add(Point obj);
	void print();
	bool isEmpty();

};