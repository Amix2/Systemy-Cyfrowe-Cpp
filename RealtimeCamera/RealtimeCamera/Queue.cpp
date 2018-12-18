#include "stdafx.h"
#include "Queue.h"

PointQueue::PointQueue(int s) {
	size = s+1;
	array = new Point[size];
}

PointQueue::~PointQueue() {
	delete(array);
}

bool PointQueue::isEmpty(){
	return takeId == addId;
}

void PointQueue::print() {
	int it = takeId;
	printf("PointQueue T:%d A:%d:\t", takeId, addId);
	while (it != addId) {
		printf("(%d,%d), ", array[it].x, array[it].y);
		it = (it + 1) % size;
	}
	printf("\n");
}

Point PointQueue::take(){
	const int id = takeId;
	takeId = (takeId+1) % size;
	return array[id];
}

bool PointQueue::add(Point obj) {
	array[addId] = obj;
	addId = (addId + 1) % size;
	return true;
}