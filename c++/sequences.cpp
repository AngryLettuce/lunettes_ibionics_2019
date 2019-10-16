#include "sequences.h"


using namespace std;

rectangleStruct arroundRectangle(){
	rectangleStruct anglesValue;
	double *singleValue;
	int tableIter = 0;
		
		//left border
		for (int i = 0; i < 400; i += RECTANGLE_STEP) {
			singleValue = getAngle(i, 0);
			anglesValue.arr[tableIter][0] = *singleValue++;
			anglesValue.arr[tableIter][1] = *singleValue;  
			tableIter++;
		}

		//bottom border
		for (int i = 1; i < 400; i += RECTANGLE_STEP) {
			singleValue = getAngle(399, i);
			anglesValue.arr[tableIter][0] = *singleValue++;
			anglesValue.arr[tableIter][1] = *singleValue;
			tableIter++;
		}

		//right border
		for (int i = 398; i >= 0; i -= RECTANGLE_STEP) {
			singleValue = getAngle(i, 399);
			anglesValue.arr[tableIter][0] = *singleValue++;
			anglesValue.arr[tableIter][1] = *singleValue;
			tableIter++;
		}

		//top border
		for (int i = 398; i > 0; i -= RECTANGLE_STEP) {
			singleValue = getAngle(0, i);
			anglesValue.arr[tableIter][0] = *singleValue++;
			anglesValue.arr[tableIter][1] = *singleValue;
			tableIter++;
		}

	return anglesValue;
}


spiralStruct spiral() {
	spiralStruct anglesValue;
	double *singleValue;
	int tableIter = 0;

	int nbOfTurn = 3;
	int pos[4] = { 200, 200, 340, 340 };

	int dp[2] = { pos[2] - pos[0], pos[3] - pos[1] };

	double R = sqrt(abs(pow(dp[0], 2) + abs(pow(dp[1], 2))));

	double phi0 = atan2(dp[1], dp[0]);

	double phi, r, x, y;

	for (int i = 0; i < SPIRAL_RESOLUTION; i++) {
		phi = i * (nbOfTurn * 2 * M_PI) / (SPIRAL_RESOLUTION - 1);
		r = i*(R / (SPIRAL_RESOLUTION -1));
		x = round(pos[0] + r * cos(phi + phi0));
		y = round(pos[1] + r * sin(phi + phi0));

		singleValue = getAngle(y, x);
		anglesValue.arr[i][0] = *singleValue++;
		anglesValue.arr[i][1] = *singleValue;
	}
	

	return anglesValue;
}


closingRectangleStruct closingRectangle() {
	closingRectangleStruct anglesValue;
	double *singleValue;
	int tableIter = 0;

	for (int closing = 0; closing < 400; closing += 40) {
		//left border
		for (int i = 0 + closing; i < 400 - closing; i += CLOSING_RECTANGLE_STEP) {
			singleValue = getAngle(i, 0 + closing);
			anglesValue.arr[tableIter][0] = *singleValue++;
			anglesValue.arr[tableIter][1] = *singleValue;
			tableIter++;
		}

		//bottom border
		for (int i = 1 + closing; i < 400 - closing; i += CLOSING_RECTANGLE_STEP) {
			singleValue = getAngle(399 - closing, i);
			anglesValue.arr[tableIter][0] = *singleValue++;
			anglesValue.arr[tableIter][1] = *singleValue;
			tableIter++;
		}

		//right border
		for (int i = 398 - closing; i >= 0 + closing; i -= CLOSING_RECTANGLE_STEP) {
			singleValue = getAngle(i, 399 - closing);
			anglesValue.arr[tableIter][0] = *singleValue++;
			anglesValue.arr[tableIter][1] = *singleValue;
			tableIter++;
		}

		//top border
		for (int i = 398 - closing; i > 0 + closing; i -= CLOSING_RECTANGLE_STEP) {
			singleValue = getAngle(0 + closing, i);
			anglesValue.arr[tableIter][0] = *singleValue++;
			anglesValue.arr[tableIter][1] = *singleValue;
			tableIter++;
		}

	}
	return anglesValue;

}



infinityStruct infinity() {
	infinityStruct anglesValue;
	double *singleValue;
	int tableIter = 0;
	double x, y;

	//left border
	for (double i = 0; i < (2 * M_PI ); i+= INFINITY_STEP) {

		x = round(((sqrt(2.0) * cos(i)) / (pow(sin(i),2.0) + 1.0)) / (sqrt(2.0) /175.0) + 200.0);
		y = round(((sqrt(2.0) * cos(i) * sin(i)) / (pow(sin(i),2.0) + 1.0)) / (0.5 / 150.0) + 200.0);
		
		cout << tableIter << endl;
		cout << x << endl;
		cout << y << endl;

		cout  << endl;
		singleValue = getAngle(int(y), int(x));
		anglesValue.arr[tableIter][0] = *singleValue++;
		anglesValue.arr[tableIter][1] = *singleValue;

		tableIter++;
	}

	return anglesValue;
}
