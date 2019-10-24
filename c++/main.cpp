#include <iostream>
#include <ctime>
#include "vectorManipulation.h"

using namespace std;
using namespace arma;
int main()
{
	rowvec maxAngles = { -3.7, 3.7, -3.5, 3.5 };
	short angleMat[CAMERA_RESOLUTION][CAMERA_RESOLUTION][2];
	recalculateAnglesMat(maxAngles, angleMat);
	
	return 0;
}