#include <iostream>
#include <ctime>
#include "vectorManipulation.h"

short angleMat[CAMERA_RESOLUTION][CAMERA_RESOLUTION][2];

using namespace std;
using namespace arma;
int main()
{
	arma_version ver;
	cout << ver.as_string() << endl;
	initAngleMat(angleMat);

	/*
	rowvec maxAngles = { -3.7, 3.7, -3.5, 4.5 };
	short angleMat[CAMERA_RESOLUTION][CAMERA_RESOLUTION][2];
	recalculateAnglesMat(maxAngles, angleMat);
	*/

	return 0;
}