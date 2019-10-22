#define _USE_MATH_DEFINES

#include "vectorManipulation.h"
#include <iostream>
#include <math.h>



#define MEMS_TILT_ANGLE 21
#define X_LASER_TO_MEMS 0
#define Y_LASER_TO_MEMS 0
#define Z_LASER_TO_MEMS 18.5
#define Z_MEMS_TO_WALL 1000
#define ZN 47.65947
#define XYZ_MATRIX_PRECISION 0.1
//#define XYZ_MATRIX_LENGTH 23989
// Laser's position relative to MEMS (in mm)
double constexpr VLM[3] = { X_LASER_TO_MEMS, Y_LASER_TO_MEMS, Z_LASER_TO_MEMS };

void recalculateAnglesMat(rowvec maxAngles, short angleMat[][CAMERA_RESOLUTION][2]) {
	mat wallCorners(4, 3);
	findWallCorners(maxAngles, wallCorners);

	mat pixMat(2, CAMERA_RESOLUTION);
	genPixMat(wallCorners, pixMat);

	int matrixSize = calcArraySize(maxAngles);
	mat XYZ_Matrix(matrixSize, 5);
	genXYZ_Matrix(maxAngles, XYZ_Matrix);

	genAnglesTable(pixMat, XYZ_Matrix, angleMat);
}


void memsNorm(double xAngle, double yAngle, rowvec& norm) {

	// MEMS default position(22 degrees, Zn = 45)
	double Zn = ZN;
	double Xn = -Zn / tan((180.0 - 90.0 + xAngle) * M_PI / 180.0);
	double Yn = -Zn / tan((180.0 - 90.0 - MEMS_TILT_ANGLE + yAngle) * M_PI / 180.0);

	norm[0] = Xn;
	norm[1] = Yn;
	norm[2] = Zn;
}

void findReflectedVector(rowvec norm, rowvec& reflectVect) {

	double lenC;
	lenC = (VLM[0] * norm[0] + VLM[1] * norm[1] + VLM[2] * norm[2]) / (pow(norm[0], 2) + pow(norm[1], 2) + pow(norm[2], 2));

	double vPar[3];
	double vPer[3];
	double vRef[3];
	double fact;
	for (int i = 0; i < 3; i++) {
		vPar[i] = lenC * norm[i];
		vPer[i] = VLM[i] - vPar[i];
		vRef[i] = vPar[i] - vPer[i];
	}

	fact = Z_MEMS_TO_WALL / vRef[2];

	for (int i = 0; i < 3; i++) {
		reflectVect[i] = vRef[i] * fact;
	}
}

double* getAngle(int xCoord, int yCoord) {
	static double XYAngles[2];

	XYAngles[0] = double(angleTable[xCoord][yCoord][0]) / 1000;
	XYAngles[1] = double(angleTable[xCoord][yCoord][1]) / 1000;

	return XYAngles;
}


void findWallCorners(rowvec maxAngles, mat& wallCorners) {
	//mat wallCorners(4, 3);
	double Xn, Yn;
	rowvec reflectVector1(3);
	rowvec reflectVector2(3);
	rowvec reflectVector3(3);
	rowvec NULC(3);
	NULC[2] = Z_MEMS_TO_WALL;

	NULC[0] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0 + maxAngles[1]));
	NULC[1] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0 - MEMS_TILT_ANGLE + maxAngles[3]));
	findReflectedVector(NULC, reflectVector1);

	NULC[0] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0));
	NULC[1] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0 - MEMS_TILT_ANGLE + maxAngles[2]));
	findReflectedVector(NULC, reflectVector2);

	NULC[0] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0 + maxAngles[0]));
	NULC[1] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0 - MEMS_TILT_ANGLE + maxAngles[3]));
	findReflectedVector(NULC, reflectVector3);

	wallCorners.row(0) = rowvec{ reflectVector1[0],reflectVector1[1], Z_MEMS_TO_WALL };
	wallCorners.row(1) = rowvec{ reflectVector1[0],reflectVector2[1], Z_MEMS_TO_WALL };
	wallCorners.row(2) = rowvec{ reflectVector3[0],reflectVector2[1], Z_MEMS_TO_WALL };
	wallCorners.row(3) = rowvec{ reflectVector3[0],reflectVector3[1], Z_MEMS_TO_WALL };
	//return wallCorners;
}

void genPixMat(mat wallCorners, mat& pixMat) {
	// add one to the resolution to get a pixMat with the correct dimensions
	// we need for exemple 401 line to get 400 intervals.
	int resolution = CAMERA_RESOLUTION + 1;
	double minX, maxX;
	double minY, maxY;

	minX = wallCorners.col(0).min();
	maxX = wallCorners.col(0).max();
	minY = wallCorners.col(1).min();
	maxY = wallCorners.col(1).max();

	rowvec width = linspace<rowvec>(minY, maxY, resolution);
	rowvec height = linspace<rowvec>(maxX, minX, resolution);

	double deltaWidth = abs(width[1] - width[0]);
	double deltaHeight = abs(height[1] - height[0]);

	width.shed_col(0);
	height.shed_col(0);
	width = width - (deltaWidth / 2);
	height = height + (deltaHeight / 2);

	pixMat.row(0) = width;
	pixMat.row(1) = height;
}


int calcArraySize(rowvec maxAngles) {
	int arraySize = ((abs(maxAngles[0]) + abs(maxAngles[1])) / XYZ_MATRIX_PRECISION + 1) * ((abs(maxAngles[2]) + abs(maxAngles[3])) / XYZ_MATRIX_PRECISION + 1);
	return ceil(arraySize);
}

void genXYZ_Matrix(rowvec maxAngles, mat& XYZ_Matrix) {

	rowvec N(3);
	rowvec reflectVector(3);
	int matrixIterator = 0;

	for (double i = maxAngles[0]; i < maxAngles[1]; i += XYZ_MATRIX_PRECISION) {
		for (double j = maxAngles[2]; j < maxAngles[3]; j += XYZ_MATRIX_PRECISION) {

			memsNorm(i, j, N);
			findReflectedVector(N, reflectVector);
			reflectVector.insert_cols(3, rowvec{ i,j });
			XYZ_Matrix.row(matrixIterator) = reflectVector;
			matrixIterator++;
			reflectVector.shed_cols(3, 4);
		}
	}
}

void genAnglesTable(mat pixMat, mat XYZ_Matrix, short angleMat[][CAMERA_RESOLUTION][2]) {
	double angles[2];
	mat modules;
	uword minIndex;
	for (int j = 0; j < CAMERA_RESOLUTION; j++) {
		for (int i = 0; i < CAMERA_RESOLUTION; i++) {
			modules = (pow(XYZ_Matrix.col(0) - pixMat(1, j), 2) + pow(XYZ_Matrix.col(1) - pixMat(0, 1), 2));
			minIndex = modules.index_min();

			//findAngles(pixMat(1,j), pixMat(0,i), XYZ_Matrix, angles);
			angleMat[j][i][0] = short(XYZ_Matrix(minIndex, 3) * 1000);
			angleMat[j][i][1] = short(XYZ_Matrix(minIndex, 4) * 1000);
			//angleMat[j][i][0] = short(angles[0] * 1000);
			//angleMat[j][i][1] = short(angles[1] * 1000);
		};
	}

}

void findAngles(double x, double y, mat XYZ_Matrix, double* angles) {
	int xTol = 8;
	int yTol = 15;

	mat tempMat;
	uvec indexInYTol;
	uvec indexInXTol;
	uvec colIndex(5);
	colIndex = { 0,1,2,3,4 };

	indexInYTol = find(abs(XYZ_Matrix.col(1) - y) < yTol);
	tempMat = XYZ_Matrix.elem(indexInYTol, colIndex);
	indexInXTol = find(abs(tempMat.col(0) - x) < xTol);
	tempMat = tempMat.elem(indexInXTol, colIndex);

	int middleIdx = ceil(indexInXTol.n_elem / 2);
	angles[0] = tempMat(middleIdx, 3);
	angles[1] = tempMat(middleIdx, 4);

}


//bool sortcol(const std::vector<double>& v1, const std::vector<double>& v2) {
//	return v1[1] < v2[1];
//}


//
//template<typename T>
//std::vector<double> linspace(T start_in, T end_in, int num_in)
//{
//	std::vector<double> linspaced;
//
//	double start = static_cast<double>(start_in);
//	double end = static_cast<double>(end_in);
//	double num = static_cast<double>(num_in);
//
//	if (num == 0) { return linspaced; }
//	if (num == 1)
//	{
//		linspaced.push_back(start);
//		return linspaced;
//	}
//
//	double delta = (end - start) / (num - 1);
//
//	for (int i = 0; i < num - 1; ++i)
//	{
//		linspaced.push_back(start + delta * i);
//	}
//	linspaced.push_back(end); // I want to ensure that start and end
//							  // are exactly the same as the input
//	return linspaced;
//}
