/*
 *  @file laser_pos_control.cpp
 *  @author Patrice Buteau et Guillaume Roux
 *  @version 15 octobre 2019
 */
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>

#include <wiringPi.h>

#include "laser_pos_control.h"
#include "lookUpTable.h"
#include "../../sequences/infinity_LUT.h"
#include "../../sequences/closingRect_LUT.h"
#include "../../sequences/rectangle_LUT.h"
#include "../../sequences/spiral_LUT.h"
#include "../../sequences/circularLoop_LUT.h"


#define MEMS_TILT_ANGLE 22
#define X_LASER_TO_MEMS 0
#define Y_LASER_TO_MEMS 0
#define Z_LASER_TO_MEMS 18.5
#define Z_MEMS_TO_WALL 1000
#define RESOLUTION_WIDTH 400
#define RESOLUTION_HEIGHT 400
#define XYZ_MATRIX_PRECISION 0.25
#define RECTANGLE_SEQUENCE_LENGTH 1596
#define SPIRAL_RESOLUTION 400
#define CLOSING_RECTANGLE_LENGTH 480
#define INFINITY_SEQUENCE_LENGTH 101
#define CIRCULAR_LOOP_SEQUENCE_LENGTH 201

using namespace std;

// Laser's position relative to MEMS (in mm)
double constexpr VLM[3] = { X_LASER_TO_MEMS, Y_LASER_TO_MEMS, Z_LASER_TO_MEMS }; // TODO: put inside class?


Laser_pos_control::Laser_pos_control() :
    laser(),
    mems(),
    button1(GPIO5_BUTTON1),
    button2(GPIO6_BUTTON2),
    button3(GPIO13_BUTTON3),
    button4(GPIO19_BUTTON4) {
    maxAngles = {-3.7, 3.7, -3.5, 4.5};
}


void Laser_pos_control::recalculateAnglesMat(short angleMat[][CAMERA_RESOLUTION][2]) {
	mat wallCorners(4, 3);
	findWallCorners(wallCorners);

	mat pixMat(2, CAMERA_RESOLUTION);
	genPixMat(wallCorners, pixMat);

	int matrixSize = calcArraySize();
	mat XYZ_Matrix(matrixSize, 5);
	genXYZ_Matrix(XYZ_Matrix);

	genAnglesTable(pixMat, XYZ_Matrix, angleMat);
}

void Laser_pos_control::memsNorm(double xAngle, double yAngle, rowvec &norm) {
	
	// MEMS default position(22 degrees, Zn = 45)
	double Zn = 45;
	double Xn = -Zn / tan((180.0 - 90.0 + xAngle) * M_PI / 180.0);
	double Yn = -Zn / tan((180.0 - 90.0 - MEMS_TILT_ANGLE + yAngle) * M_PI / 180.0);

	norm[0] = Xn;
	norm[1] = Yn;
	norm[2] = Zn;
}

void Laser_pos_control::findReflectedVector(rowvec norm, rowvec &reflectVect) {

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

float *Laser_pos_control::getAngles(int xCoord, int yCoord) {
	static float XYAngles[2];

	XYAngles[0] = float(angleTable[xCoord][yCoord][0]) / 1000;
	XYAngles[1] = float(angleTable[xCoord][yCoord][1]) / 1000;

	return XYAngles;
}

void Laser_pos_control::findWallCorners(mat &wallCorners) {
	//mat wallCorners(4, 3);
	double Xn, Yn;
	rowvec reflectVector1(3);
	rowvec reflectVector2(3);
	rowvec reflectVector3(3);
	rowvec NULC(3);
	NULC[2] = Z_MEMS_TO_WALL;

	NULC[0] = -Z_MEMS_TO_WALL / tan(M_PI/180 * (180.0 - 90.0 + maxAngles[1]));
	NULC[1] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0 - MEMS_TILT_ANGLE + maxAngles[3]));
	findReflectedVector(NULC, reflectVector1);

	NULC[0] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0));
	NULC[1] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0 - MEMS_TILT_ANGLE + maxAngles[2]));
	findReflectedVector(NULC, reflectVector2);

	NULC[0] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0 + maxAngles[0]));
	NULC[1] = -Z_MEMS_TO_WALL / tan(M_PI / 180 * (180.0 - 90.0 - MEMS_TILT_ANGLE + maxAngles[3]));
	findReflectedVector(NULC, reflectVector3);

	wallCorners.row(0) = rowvec {reflectVector1[0],reflectVector1[1], Z_MEMS_TO_WALL };
	wallCorners.row(1) = rowvec {reflectVector1[0],reflectVector2[1], Z_MEMS_TO_WALL };
	wallCorners.row(2) = rowvec {reflectVector3[0],reflectVector2[1], Z_MEMS_TO_WALL };
	wallCorners.row(3) = rowvec {reflectVector3[0],reflectVector3[1], Z_MEMS_TO_WALL };
	//return wallCorners;
}

void Laser_pos_control::genPixMat(mat wallCorners, mat &pixMat) {
	// add one to the resolution to get a pixMat with the correct dimensions
	// we need for exemple 401 line to get 400 intervals.
	int resolution = CAMERA_RESOLUTION + 1;
	double minX, maxX;
	double minY, maxY;

	minX = wallCorners.col(0).min();
	maxX = wallCorners.col(0).max();
	minY = wallCorners.col(1).min();
	maxY = wallCorners.col(1).max();

	rowvec width  = linspace<rowvec>(minY, maxY, resolution);
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


int Laser_pos_control::calcArraySize() {
	int arraySize = ((abs(maxAngles[0]) + abs(maxAngles[1]))/ XYZ_MATRIX_PRECISION + 1) * ((abs(maxAngles[2]) + abs(maxAngles[3])) / XYZ_MATRIX_PRECISION + 1);
	return ceil(arraySize);
}

void Laser_pos_control::genXYZ_Matrix(mat &XYZ_Matrix) {

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
			reflectVector.shed_cols(3,4);
		}
	}
}

void Laser_pos_control::genAnglesTable(mat pixMat, mat XYZ_Matrix, short angleMat[][CAMERA_RESOLUTION][2]) {
	double angles[2];
	for (int j = 0; j < CAMERA_RESOLUTION; j++) {
		for (int i = 0; i < CAMERA_RESOLUTION; i++) {
			findAngles(pixMat(1,j), pixMat(0,i), XYZ_Matrix, angles);
			angleMat[j][i][0] = short(angles[0] * 1000);
			angleMat[j][i][1] = short(angles[1] * 1000);
		};
	}
}

void Laser_pos_control::findAngles(double x, double y, mat XYZ_Matrix, double *angles) {
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
	angles[0] = tempMat(middleIdx,3);
	angles[1] = tempMat(middleIdx,4);

}


void Laser_pos_control::draw_rectangle(int time_delay) {
	float *angles;
	for (int i = 0; i < RECTANGLE_SEQUENCE_LENGTH; i++) {
		angles = getAngles(rectangle_LUT[i][0], rectangle_LUT[i][1]);
		mems.send_angle_x(*angles++);
		mems.send_angle_y(*angles);
		delay(time_delay);
	}
}

void Laser_pos_control::draw_Closingrectangle(int time_delay) {
	float *angles;
	for (int i = 0; i < CLOSING_RECTANGLE_LENGTH; i++) {
		angles = getAngles(closingRect_LUT[i][0], closingRect_LUT[i][1]);
		mems.send_angle_x(*angles++);
		mems.send_angle_y(*angles);
		delay(time_delay);
	}
}

void Laser_pos_control::draw_spiral(int time_delay) {
	float *angles;
	for (int i = 0; i < SPIRAL_RESOLUTION; i++) {
		angles = getAngles(spiral_LUT[i][0], spiral_LUT[i][1]);
		mems.send_angle_x(*angles++);
		mems.send_angle_y(*angles);
		delay(time_delay);
	}
}

void Laser_pos_control::draw_infinity(int time_delay) {
	float *angles;
	for (int i = 0; i < INFINITY_SEQUENCE_LENGTH; i++) {
		angles = getAngles(infinity_LUT[i][0], infinity_LUT[i][1]);
		mems.send_angle_x(*angles++);
		mems.send_angle_y(*angles);
		delay(time_delay);
	} 
}

void Laser_pos_control::draw_circluarLoop(int time_delay){
	float *angles;
	for (int i = 0; i < CIRCULAR_LOOP_SEQUENCE_LENGTH; i++) {
		angles = getAngles(circularLoop_LUT[i][0], circularLoop_LUT[i][1]);
		mems.send_angle_x(*angles++);
		cout << *angles << endl;
		mems.send_angle_y(*angles);
		delay(time_delay);
	} 
}

float* Laser_pos_control::manual_mode() {
    const float delta_angle = 0.02;
    const int wait_delay = 100;
    float momentum = delta_angle;
    static float angles[2];
    int axis = 0;
    float angle_x = 0;
    float angle_y = 0;
    while(1) {
        if(button4.scan_button() == PRESSED) {
            angles[0] = angle_x;
            angles[1] = angle_y;
            return angles;
        }
        if(button3.scan_button() == PRESSED) {
		cout << "BUTTON 3!" << endl;
            axis ^= 1;
        }
        if(axis) {
            if(button1.scan_button() == HELD_DOWN || button1.scan_button() == PRESSED) {
                //cout << "BUTTON 1!" << endl;
		angle_x += momentum;
		cout << angle_x << endl;
                mems.send_angle_x(angle_x);
                momentum += delta_angle;
            }
            else if(button2.scan_button() == HELD_DOWN || button2.scan_button() == PRESSED) {
                //cout << "BUTTON 2!" << endl;
		angle_x -= momentum; 
                mems.send_angle_x(angle_x);
                momentum += delta_angle;
            }
            else {
                momentum = delta_angle;
            }
        }
        else {
            if(button1.scan_button() == HELD_DOWN || button1.scan_button() == PRESSED) {
                //cout << "BUTTON 1!" << endl;
		angle_y += momentum; 
                mems.send_angle_y(angle_y);
                momentum += delta_angle;
            }
            else if(button2.scan_button() == HELD_DOWN || button2.scan_button() == PRESSED) {
                //cout << "BUTTON 2!" << endl;
		angle_y -= momentum; 
                mems.send_angle_y(angle_y);
                momentum += delta_angle;
            }
            else {
                momentum = delta_angle;
            }
        }
        delay(wait_delay);
    }
}

void Laser_pos_control::set_max_angles() {
    int corner = 0;

    while(corner <= 1) {
        float *angles;
        angles = manual_mode();
        if(corner == 0) {
            maxAngles[0] = angles[0];
            maxAngles[3] = angles[1];
        }
        else {
            maxAngles[1] = angles[0];
            maxAngles[2] = angles[1];
        }
        corner++;
    }
}
