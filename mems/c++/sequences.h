#pragma once

#include "vectorManipulation.h"

#include <cmath>
#include <iostream>

#define RECTANGLE_SEQUENCE_LENGTH 1596
#define RECTANGLE_STEP 1

#define SPIRAL_RESOLUTION 400

#define CLOSING_RECTANGLE_LENGHT 480
#define CLOSING_RECTANGLE_STEP 10

#define INFINITY_SEQUENCE_LENGTH 101
#define INFINITY_STEP M_PI / 50.0


struct rectangleStruct {
	double arr[RECTANGLE_SEQUENCE_LENGTH][2];
};

struct spiralStruct {
	double arr[SPIRAL_RESOLUTION][2];
};

struct closingRectangleStruct {
	double arr[CLOSING_RECTANGLE_LENGHT][2];
};

struct infinityStruct {
	double arr[INFINITY_SEQUENCE_LENGTH][2];
};

rectangleStruct arroundRectangle();

spiralStruct spiral();

closingRectangleStruct closingRectangle();

infinityStruct infinity();