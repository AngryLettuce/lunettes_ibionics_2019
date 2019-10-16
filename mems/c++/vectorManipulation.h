#pragma once

#include "lookUpTable.h"
#include <armadillo>
#include <vector>

#define CAMERA_RESOLUTION 400

using namespace arma;

void recalculateAnglesMat(rowvec maxAngles, short angleMat[][CAMERA_RESOLUTION][2]);

void memsNorm(double xAngle, double yAngle, rowvec& norm);

void findReflectedVector(rowvec norm, rowvec& reflectVect);

void findWallCorners(rowvec maxAngles, mat& wallCorners);

void genPixMat(mat wallCorners, mat& pixMat);

double* getAngle(int xCoord, int yCoord);

void genXYZ_Matrix(rowvec maxAngles, mat& XYZ_Matrix);

void genAnglesTable(mat pixMat, mat XYZ_Matrix, short angleMat[][CAMERA_RESOLUTION][2]);
void findAngles(double x, double y, mat XYZ_Matrix, double* angles);

int calcArraySize(rowvec maxAngles);

//// Driver function to sort the 2D vector 
//// on basis of a particular column 
//bool sortcol(const std::vector<double>& v1, const std::vector<double>& v2);
//
//template<typename T>
//std::vector<double> linspace(T start_in, T end_in, int num_in);