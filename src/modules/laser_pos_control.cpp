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
#include "../../mathMems/lookUpTable.h"
#include "../../sequences/infinity_LUT.h"
#include "../../sequences/closingRect_LUT.h"
#include "../../sequences/rectangle_LUT.h"
#include "../../sequences/spiral_LUT.h"
#include "../../sequences/circularLoop_LUT.h"

#define MEMS_TILT_ANGLE 21
#define X_LASER_TO_MEMS 0
#define Y_LASER_TO_MEMS 0
#define Z_LASER_TO_MEMS -1
#define Z_MEMS_TO_WALL 1000
#define XYZ_MATRIX_PRECISION 0.1
#define RECTANGLE_SEQUENCE_LENGTH 80
#define SPIRAL_RESOLUTION 100
#define CLOSING_RECTANGLE_LENGTH 240
#define INFINITY_SEQUENCE_LENGTH 101
#define CIRCULAR_LOOP_SEQUENCE_LENGTH 201

using namespace std;

Laser_pos_control::Laser_pos_control() :
    laser(),
    mems(),
    button1(GPIO7_BUTTON1),
    button2(GPIO6_BUTTON2),
    button3(GPIO5_BUTTON3),
    button4(GPIO4_BUTTON4) {
    maxAngles = {-3.7, 3.7, -3.5, 4.5};
	VLM = {X_LASER_TO_MEMS, Y_LASER_TO_MEMS, Z_LASER_TO_MEMS};
}


void Laser_pos_control::recalculateAnglesMat() {
	mat wallCorners(4, 3);
	findWallCorners(wallCorners);

	mat pixMat(2, CAMERA_RESOLUTION);
	genPixMat(wallCorners, pixMat);

	int matrixSize = calcArraySize();
	mat XYZ_Matrix(matrixSize, 5);
	genXYZ_Matrix(XYZ_Matrix);

	genAnglesTable(pixMat, XYZ_Matrix, angleTable);
}


void Laser_pos_control::findWallCorners(mat &wallCorners) {
	float x, y;
	//top left (max aX, min aY)
	angle2XY(maxAngles[1], maxAngles[2], x, y);
	wallCorners.row(0) = rowvec{ x, y, Z_MEMS_TO_WALL };
	//left bottom (min aX, min aY)
	angle2XY(maxAngles[0], maxAngles[2], x, y);
	wallCorners.row(1) = rowvec{ x, y, Z_MEMS_TO_WALL };
	//right bottom (min aX, max aY)
	angle2XY(maxAngles[0], maxAngles[3], x, y);
	wallCorners.row(2) = rowvec{ x, y, Z_MEMS_TO_WALL };
	//top right (max aX, max aY)
	angle2XY(maxAngles[1], maxAngles[3], x, y);
	wallCorners.row(3) = rowvec{ x, y, Z_MEMS_TO_WALL };
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

float* Laser_pos_control::getAngles(int xCoord, int yCoord) {
	static float XYAngles[2];

	XYAngles[0] = float(angleTable[xCoord][yCoord][0]) / 1000;
	XYAngles[1] = float(angleTable[xCoord][yCoord][1]) / 1000;

	return XYAngles;
}


int Laser_pos_control::calcArraySize() {
	int arraySize = ((abs(maxAngles[0]) + abs(maxAngles[1]))/ XYZ_MATRIX_PRECISION + 1) * ((abs(maxAngles[2]) + abs(maxAngles[3])) / XYZ_MATRIX_PRECISION + 1);
	return ceil(arraySize);
}

void Laser_pos_control::genXYZ_Matrix(mat &XYZ_Matrix) {
	rowvec reflectVector(3);
	int matrixIterator = 0;
	float x, y;
	for (double i = maxAngles[0]; i < maxAngles[1]; i += XYZ_MATRIX_PRECISION) {
		for (double j = maxAngles[2]; j < maxAngles[3]; j += XYZ_MATRIX_PRECISION) {
			angle2XY(i, j, x, y);
			XYZ_Matrix.row(matrixIterator) = rowvec{ x, y, Z_MEMS_TO_WALL, i, j };
			matrixIterator++;
		}
	}
}

void Laser_pos_control::genAnglesTable(mat pixMat, mat XYZ_Matrix, short angleMat[][CAMERA_RESOLUTION][2]) {
	mat modules;
	uword minIndex;
	for (int j = 0; j < CAMERA_RESOLUTION; j++) {
		for (int i = 0; i < CAMERA_RESOLUTION; i++) {
			modules = (pow(XYZ_Matrix.col(0) - pixMat(0, j), 2) + pow(XYZ_Matrix.col(1) - pixMat(1, i), 2));
			minIndex = modules.index_min();
			angleMat[i][j][0] = short(XYZ_Matrix(minIndex, 3) * 1000);
			angleMat[i][j][1] = short(XYZ_Matrix(minIndex, 4) * 1000);
		};
	}
}

/*
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
*/

void Laser_pos_control::angle2XY(float aX, float aY, float &x, float &y) {
	float theta_x = deg2rad(aX);
	float theta_y = deg2rad(aY);
	float alpha = deg2rad(MEMS_TILT_ANGLE) - theta_y + M_PI / 2;
	float beta = theta_x;
	float zn = sin(alpha)*cos(beta);
	float xn = cos(alpha)*cos(beta);
	float yn = sin(beta);
	rowvec N = { xn, yn, zn };
	N = N / sqrt(pow(xn, 2) + pow(yn, 2) + pow(zn, 2));

	//reflexion
	rowvec reflexion = VLM - 2 * (VLM[0] * N[0] + VLM[1] * N[1] + VLM[2] * N[2]) * N;
	float k = Z_MEMS_TO_WALL / reflexion[2];
	x = k * reflexion[0];
	y = k * reflexion[1];

}

double Laser_pos_control::deg2rad(float angle) {
	return (angle * M_PI / 180);
}

void Laser_pos_control::draw_rectangle(int time_delay) {
	float *angles;
	for (int i = 0; i < RECTANGLE_SEQUENCE_LENGTH; i++) {
		angles = getAngles(rectangle_LUT[i][0], rectangle_LUT[i][1]);
		mems.send_angle_x(*angles++);
		//cout << *angles << endl;
		mems.send_angle_y(*angles);
		delay(time_delay);
	}
}

void Laser_pos_control::draw_Closingrectangle(int time_delay) {
	float *angles;
	for (int i = 0; i < CLOSING_RECTANGLE_LENGTH; i++) {
		angles = getAngles(closingRect_LUT[i][0], closingRect_LUT[i][1]);
		mems.send_angle_x(*angles++);
		//cout << *angles << endl;
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
		//cout << *angles << endl;
		mems.send_angle_y(*angles);
		delay(time_delay);
	}
}

void Laser_pos_control::draw_circluarLoop(int time_delay){
	float *angles;
	for (int i = 0; i < CIRCULAR_LOOP_SEQUENCE_LENGTH; i++) {
		angles = getAngles(circularLoop_LUT[i][0], circularLoop_LUT[i][1]);
		mems.send_angle_x(*angles++);
		//cout << *angles << endl;
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

	mems.send_angle_x(0);
	mems.send_angle_y(0);

    while(1) {
        if(button4.scan_button() == PRESSED) { // Exit
            angles[0] = angle_x;
            angles[1] = angle_y;
            return angles;
        }
        if(button3.scan_button() == PRESSED) { // Change of axis
            axis ^= 1;
        }
        if(axis) {
            if(button1.scan_button() == HELD_DOWN || button1.scan_button() == PRESSED) {
				// Increase x angle
				angle_x += momentum;
				angle_x = mems.saturate_angle(angle_x);
				cout << angle_x << endl;
                mems.send_angle_x(angle_x);
                momentum += delta_angle;
            } else if(button2.scan_button() == HELD_DOWN || button2.scan_button() == PRESSED) {
				// Decrease x angle
				angle_x -= momentum;
				angle_x = mems.saturate_angle(angle_x);
				cout << angle_x << endl;
                mems.send_angle_x(angle_x);
                momentum += delta_angle;
            } else { // reset momentum
                momentum = delta_angle;
            }
        } else {
            if(button1.scan_button() == HELD_DOWN || button1.scan_button() == PRESSED) {
				// Increase y angle
				angle_y += momentum;
				angle_y = mems.saturate_angle(angle_y);
				cout << angle_y << endl;
                mems.send_angle_y(angle_y);
                momentum += delta_angle;
			} else if(button2.scan_button() == HELD_DOWN || button2.scan_button() == PRESSED) {
				// Decrease y angle
				angle_y -= momentum;
				angle_y = mems.saturate_angle(angle_y);
				cout << angle_y << endl;
                mems.send_angle_y(angle_y);
                momentum += delta_angle;
            } else { // reset momentum
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
        if(corner == 0) { // Up left corner
            maxAngles[1] = angles[0]; // maxX
            maxAngles[2] = angles[1]; // minY
        } else { // Down right corner
            maxAngles[0] = angles[0]; // minX
            maxAngles[3] = angles[1]; // maxY
        }
        corner++;
    }
    maxAngles.print();
}

void Laser_pos_control::send_pos(int x, int y){
	float *XYAngles;
	XYAngles = getAngles(x,y);
	mems.send_angles(XYAngles[0], XYAngles[1]);
}
