#include <iostream>
#include <ctime>
#include "vectorManipulation.h"

short angleMat[CAMERA_RESOLUTION][CAMERA_RESOLUTION][2];

using namespace std;
using namespace arma;
int main()
{
	mat gridX(4, 5, fill::ones);
	mat gridY(4, 5, fill::zeros);
	initAngleMat(angleMat);

	saveAnglePoints(gridX, gridY, "exportData.txt");
	loadAnglePoints(gridY, gridX, "exportData.txt");


	/*
	rowvec maxAngles = { -3.7, 3.7, -3.5, 4.5 };
	short angleMat[CAMERA_RESOLUTION][CAMERA_RESOLUTION][2];
	recalculateAnglesMat(maxAngles, angleMat);
	*/

	return 0;
}