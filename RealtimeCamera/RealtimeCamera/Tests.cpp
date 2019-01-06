#pragma once
#include "stdafx.h"
#include "Line.h"
#include "Stripe.h"
#include "MathFunctions.h"
#include "ImageProcessing.h"
#include "Queue.h"
#include <windows.h>
#include "LineFinder.h"
using namespace std;

void lineTest() {
	cout<<"Line test"<<endl;
	Point A(1, 1), B(1, 2), C(2,1), D(2,3), E(3,3);
	Line l1(A, A), l2(A, B), l3(A, C), l4(A, D), l5(A,E);
	cout << "Line l1(A, A), l2(A, B), l3(A, C), l4(A, D);" << endl;


	l1.print();
	l2.print();
	l3.print();
	l4.print();
	l5.print();
	cout << endl;

	cout << l4.isAbove(A, E) << l4.isBelow(A, E) << endl;
	/*
	printf("%d\t%d\t%d\t%d\t%d\n",
		toDeg(angleCalculate(l1)), toDeg(angleCalculate(l2)), toDeg(angleCalculate(l3)), toDeg(angleCalculate(l4)), toDeg(angleCalculate(l5)));
	printf("%d\t%d\n", toDeg(l5.getAngleCloseTo(0)), toDeg(l5.getAngleCloseTo(2*PI)));
	Line l10(A, 0), l11(A, PI), l12(A, PI / 2);
	l10.print();
	l11.print();
	l12.print();

	Line l6 = createLineDistanceFromPointCloseToPoint(A, E, 1.0, B);
	l6.print();
	cout << l6.distanceFrom(E) << endl;
	Line l7 = createLineDistanceFromPointCloseToPoint(A, B, 1.0, E);
	l7.print();
	cout << l7.distanceFrom(B) << endl;
	*/
}

void stripeTest() {
	cout << "\n\nStripe test" << endl;
	Point A(1, 1), B(2, 1), C(3,3), D(4,4), E(7,2), F(2,1);
	Stripe str(A, B, 1);
	str.print();
	cout << "add1 1 ::" << str.tryAdd(C) << endl;
	str.add(C);
	str.print();
	cout << "################" << endl;
	Stripe str2(A, C, 1);
	str2.print();
	cout << "Add: 1,0 :::: "<<str2.tryAdd(D) << str2.tryAdd(E) << endl;
	str2.add(D);
	str2.print();
	cout << "################" << endl;
	Stripe str3(A, B, 1);
	cout << "Add 1 :::" << str3.tryAdd(F) << endl;
}

void angleTest() {
	cout << "\n\nAngle test" << endl;
	double a1 = 1, a2 = 2, a3 = 3;
	cout << angleCompare(a2, a3) << angleCompare(a2, a1) << endl;
	cout << angleIsBetween(a2, a3, a1) << endl;
	cout << "angleAverage 1, -2,.. ::::" << angleAverage(0, 2) << angleAverage(3, -2) << endl;
}

void miscTest() {
	cout << "distance" << endl;
	cout << distance({ 0,0 }, { -1,1 }) << endl;
}

void queueTest() {
	cout << "\n\nqueueTest test" << endl;
	Point A(1, 1), B(2, 2), C(3, 3), D(4, 4), E(5,5), F(6,6);
	PointQueue qu(5);
	qu.print();
	cout << "queue empty 1::: " << qu.isEmpty() << endl;
	qu.add(A);
	qu.print();
	cout << "queue empty 0::: " << qu.isEmpty() << endl;
	qu.add(B);
	qu.add(C);
	qu.add(D);
	qu.add(E);
	qu.print();
	qu.take();
	qu.take();
	qu.take();
	qu.add(A);
	qu.add(B);
	qu.add(C);
	qu.print();
	cout << "isfull 1:::" << qu.isFull() << endl;
	qu.take();
	qu.print();
	cout << "isfull 0:::" << qu.isFull() << endl;
}

void finderTest() {
	cout << "\n\nfinderTest test" << endl;
	Frame frame;
	frame.content = new int[HEIGHT * WIDTH];
	for (int i = 0; i < 3 * WIDTH; i++) frame[i] = 0;
	for (int i = 5; i < 12; i++) {
		Point px(i, 1);
		setAllColors(&(frame[px]), 100, 100, 100);
	}
	int dir1 = isGodStartingPoint(frame, { 7, 1 });
	int dir2 = 3;
	cout << getMoveFomDirection(3) << neighbours({ 10,10 }, 3).x<< neighbours({ 10,10 }, 3).y<< endl;
	cout << dir1 << endl;
	cout << getMoveFomDirection(dir1/10) << endl;
	Point p2 = performMoveFromDirection({ 10,10 }, dir2);
	cout << p2.x << "," << p2.y << endl;

	Point pEnd = traceLine(frame, { 7,1 }, { 8,1 }, NULL);
	cout << pEnd.x << "," << pEnd.y << endl;

	Point bestEnd = findNextBestStartingPoint(frame, { 7,1 }, dir1);
	cout << "bestEnd  " << bestEnd.x << "," << bestEnd.y << endl;

	//Point p3 = findEndOfContinueLine(frame, { 7,1 });
	//Point p4 = findEndOfContinueLine(frame, { 7,1 });
	//cout << "findEndOfContinueLine:  " << p3.x << "," << p3.y << "   :::   " << p4.x << "," << p4.y << endl;
	
	cout << endl << "FULL TEST" << endl;
	initVertexTab();
	findLanes(frame);
	printVertex();
	
	delete(frame.content);
}

void doAllTests() {
	angleTest();
	miscTest();
	lineTest();
	stripeTest();
	queueTest();
	finderTest();
	Sleep(1000000000000000);
}