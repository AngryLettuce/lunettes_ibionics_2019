#define _USE_MATH_DEFINES


#include <iostream>
#include <math.h>
#include <fstream>
#include <opencv2/core/mat.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>

#include "vectorManipulation.h"
#include "anglesPoints.h"


#define MEMS_TILT_ANGLE 21
#define X_LASER_TO_MEMS 0
#define Y_LASER_TO_MEMS 0
#define Z_LASER_TO_MEMS -1
#define Z_MEMS_TO_WALL 1000
//#define ZN 47.65947
#define XYZ_MATRIX_PRECISION 0.1

#define GRID_POINTS_X 5
#define GRID_POINTS_Y 4
//#define XYZ_MATRIX_LENGTH 23989
// Laser's position relative to MEMS (in mm)
rowvec VLM = { X_LASER_TO_MEMS, Y_LASER_TO_MEMS, Z_LASER_TO_MEMS };

void initAngleMat_CV2(short angleMat[][CAMERA_RESOLUTION][2]) {
	cv::Mat srcX, dstX;
	cv::Mat srcY, dstY;
	//cv::Mat map_x, map_y;

	srcX.create(GRID_POINTS_Y, GRID_POINTS_X, CV_32FC1);
	srcY.create(GRID_POINTS_Y, GRID_POINTS_X, CV_32FC1);

	for (int i = 0; i < GRID_POINTS_Y; i++) {
		for (int j = 0; j < GRID_POINTS_X; j++) {
			srcX.at<float>(i, j) = anglePointsX.at(i, j);
			srcY.at<float>(i, j) = anglePointsY.at(i, j);
		}
	}
	
	dstX.create(CAMERA_RESOLUTION, CAMERA_RESOLUTION, CV_32FC1);
	dstY.create(CAMERA_RESOLUTION, CAMERA_RESOLUTION, CV_32FC1);

	resize(srcX, dstX, dstX.size(), 0, 0, cv::INTER_LINEAR);
	resize(srcY, dstY, dstY.size(), 0, 0, cv::INTER_LINEAR);

	
	for (int i = 0; i < CAMERA_RESOLUTION; i++) {
		for (int j = 0; j < CAMERA_RESOLUTION; j++) {
			angleMat[i][j][0] = (short) (dstX.at<float>(i, j) * 1000);
			angleMat[i][j][1] = (short) (dstY.at<float>(i, j) * 1000);
		}
	}
	
	
}



void initAngleMat(short angleMat[][CAMERA_RESOLUTION][2]) {
	vec X = linspace(0, CAMERA_RESOLUTION - 1, 5);
	vec Y = linspace(0, CAMERA_RESOLUTION - 1, 5);
	vec Xi = linspace(0, CAMERA_RESOLUTION - 1, CAMERA_RESOLUTION);
	vec Yi = linspace(0, CAMERA_RESOLUTION - 1, CAMERA_RESOLUTION);
	mat ZiX;
	mat ZiY;

	interp2(X, Y, anglePointsX, Xi, Yi, ZiX);
	interp2(X, Y, anglePointsY, Xi, Yi, ZiY);


	for (int i = 0; i < CAMERA_RESOLUTION; i++) {
		for (int j = 0; j < CAMERA_RESOLUTION; j++) {
			angleMat[i][j][0] = ZiX.at(i, j) * 1000;
			angleMat[i][j][1] = ZiY.at(j, i) * 1000;
		}
	}
}

void saveAnglePoints(mat gridX, mat gridY, const char* fileName) {
	std::ofstream myfile;
	myfile.open(fileName);

	for (int i = 0; i < gridX.n_rows; i++) {
		for (int j = 0; j < gridX.n_cols; j++) {
			myfile << gridX.at(i, j);
			if (j == gridX.n_cols - 1) {
				myfile << endl;
			}
			else {
				myfile << " ";
			}
		}
	}
	for (int i = 0; i < gridY.n_rows; i++) {
		for (int j = 0; j < gridY.n_cols; j++) {
			myfile << gridY.at(i, j);
			if (j == gridY.n_cols - 1) {
				myfile << endl;
			}
			else {
				myfile << " ";
			}
		}
	}
	myfile.close();	
}

void loadAnglePoints(mat& gridX, mat& gridY, const char* fileName) {
	std::ifstream myfile;
	myfile.open(fileName);

	for (int i = 0; i < GRID_POINTS_Y; i++) {
		for (int j = 0; j < GRID_POINTS_X; j++) {
			myfile >> gridX.at(i, j);
		}
	}

	for (int i = 0; i < GRID_POINTS_Y; i++) {
		for (int j = 0; j < GRID_POINTS_X; j++) {
			myfile >> gridY.at(i, j);
		}
	}
	myfile.close();
}

/*
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
*/

/*
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
*/
/*
double* getAngle(int xCoord, int yCoord) {
	static double XYAngles[2];

	XYAngles[0] = double(angleTable[xCoord][yCoord][0]) / 1000;
	XYAngles[1] = double(angleTable[xCoord][yCoord][1]) / 1000;

	return XYAngles;
}
*/
/*
void findWallCorners(rowvec maxAngles, mat& wallCorners) {

	float x, y;
	//top left (min aX, min aY)
	angle2XY(maxAngles[0], maxAngles[2], x, y);
	wallCorners.row(0) = rowvec{ x, y, Z_MEMS_TO_WALL };
	//left bottom (min aX, max aY)
	angle2XY(maxAngles[0], maxAngles[3], x, y);
	wallCorners.row(1) = rowvec{ x, y, Z_MEMS_TO_WALL };
	//right bottom (max aX, max aY)
	angle2XY(maxAngles[1], maxAngles[3], x, y);
	wallCorners.row(2) = rowvec{ x, y, Z_MEMS_TO_WALL };
	//top left (max aX, min aY)
	angle2XY(maxAngles[1], maxAngles[2], x, y);
	wallCorners.row(3) = rowvec{ x, y, Z_MEMS_TO_WALL };


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

	rowvec width = linspace<rowvec>(minX, maxX, resolution);
	rowvec height = linspace<rowvec>(maxY, minY, resolution);

	double deltaWidth = abs(width[1] - width[0]);
	double deltaHeight = abs(height[1] - height[0]);

	width.shed_col(0);
	height.shed_col(0);
	width = width - (deltaWidth / 2);
	height = height + (deltaHeight / 2);

	pixMat.row(0) = width;
	pixMat.row(1) = height;

}
*/

/*
int calcArraySize(rowvec maxAngles) {
	int arraySize = ((abs(maxAngles[0]) + abs(maxAngles[1])) / XYZ_MATRIX_PRECISION + 1) * ((abs(maxAngles[2]) + abs(maxAngles[3])) / XYZ_MATRIX_PRECISION + 1);
	return ceil(arraySize);
}

void genXYZ_Matrix(rowvec maxAngles, mat& XYZ_Matrix) {

	//rowvec N(3);
	rowvec reflectVector(3);
	int matrixIterator = 0;
	float x, y;
	for (double i = maxAngles[0]; i < maxAngles[1]; i += XYZ_MATRIX_PRECISION) {
		for (double j = maxAngles[2]; j < maxAngles[3]; j += XYZ_MATRIX_PRECISION) {

			//memsNorm(i, j, N);
			//findReflectedVector(N, reflectVector);
			//reflectVector.insert_cols(3, rowvec{ i,j });
			angle2XY(i, j, x, y);
			XYZ_Matrix.row(matrixIterator) = rowvec{ x, y, Z_MEMS_TO_WALL, i, j };
			//XYZ_Matrix.row(matrixIterator) = reflectVector;
			matrixIterator++;
			//reflectVector.shed_cols(3, 4);
		}
	}
}
*/
/*
void genAnglesTable(mat pixMat, mat XYZ_Matrix, short angleMat[][CAMERA_RESOLUTION][2]) {
	//double angles[2];
	mat modules;
	uword minIndex;
	for (int j = 0; j < CAMERA_RESOLUTION; j++) {
		for (int i = 0; i < CAMERA_RESOLUTION; i++) {
			modules = (pow(XYZ_Matrix.col(0) - pixMat(0, j), 2) + pow(XYZ_Matrix.col(1) - pixMat(1, i), 2));
			minIndex = modules.index_min();
			//findAngles(pixMat(1,j), pixMat(0,i), XYZ_Matrix, angles);
			angleMat[i][j][0] = short(XYZ_Matrix(minIndex, 3) * 1000);
			angleMat[i][j][1] = short(XYZ_Matrix(minIndex, 4) * 1000);
			//angleMat[j][i][0] = short(angles[0] * 1000);
			//angleMat[j][i][1] = short(angles[1] * 1000);
		};
	}

}
*/
/*
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
*/
/*
void angle2XY(float aX, float aY, float &x, float &y) {
	float theta_x = deg2rad(aX);
	float theta_y = deg2rad(aY);
	float alpha = deg2rad(MEMS_TILT_ANGLE) - theta_x + M_PI / 2;
	float beta =  -theta_y;
	float zn = sin(alpha)*cos(beta);
	float xn = cos(alpha)*cos(beta);
	float yn = sin(beta);
	rowvec N = { xn, yn, zn };
	N = N / sqrt(pow(xn, 2) + pow(yn, 2) + pow(zn, 2));

	//reflexion
	rowvec reflexion = VLM-2*(VLM[0] *N[0] + VLM[1] * N[1] + VLM[2] * N[2]) * N;
	float k = Z_MEMS_TO_WALL / reflexion[2];
	x = k * reflexion[0];
	y = k * reflexion[1];

}

double deg2rad(float angle) {
	return (angle * M_PI / 180);
}

*/

