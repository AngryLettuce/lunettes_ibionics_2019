/*
 *  @file laser_pos_control.cpp
 *  @author Patrice Buteau et Guillaume Roux
 *  @version 15 octobre 2019
 */
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <math.h>

#ifdef __arm__
#include <wiringPi.h>
#endif

#include "laser_pos_control.h"
//#include "anglesPoints2.h"
#include "getch.h"
//#include "../../mathMems/interpolationBilineaire/lookUpTable.h"
#include "../../sequences/infinity_LUT.h"
#include "../../sequences/closingRect_LUT.h"
#include "../../sequences/rectangle_LUT.h"
#include "../../sequences/spiral_LUT.h"
#include "../../sequences/circularLoop_LUT.h"

//#define MEMS_TILT_ANGLE 21
//#define X_LASER_TO_MEMS 0
//#define Y_LASER_TO_MEMS 0
//#define Z_LASER_TO_MEMS -1
//#define Z_MEMS_TO_WALL 1000
//#define XYZ_MATRIX_PRECISION 0.1
#define RECTANGLE_SEQUENCE_LENGTH 80
#define SPIRAL_RESOLUTION 100
#define CLOSING_RECTANGLE_LENGTH 240
#define INFINITY_SEQUENCE_LENGTH 101
#define CIRCULAR_LOOP_SEQUENCE_LENGTH 201

//#define ANGLE_POINTS_FILENAME_H "modules/anglesPoints2.h"
//#define ANGLE_POINTS_FILENAME_TXT "modules/anglesPoints.txt"

#ifdef __arm__
	#define ANGLE_POINTS_FILENAME_TXT "/home/pi/Desktop/s8ibionics/src/modules/anglesPoints.txt"
#endif

#ifndef __arm__
	#define ANGLE_POINTS_FILENAME_TXT "anglesPoints.txt"
#endif

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_SPACE 32
#define KB_ENTER 13
#define KB_Q 113
#define KB_W 119
#define KB_A 97
#define KB_S 115
#define KB_D 100

using namespace std;

Laser_pos_control::Laser_pos_control() :

    laser(),
    mems(),
    button1(GPIO7_BUTTON1),
    button2(GPIO6_BUTTON2),
    button3(GPIO5_BUTTON3),
    button4(GPIO4_BUTTON4) {
		/*
#ifdef __arm__
	gridPointsX.create(Y_ANGLES_GRID_POINTS, X_ANGLES_GRID_POINTS, CV_64F);
	gridPointsY.create(Y_ANGLES_GRID_POINTS, X_ANGLES_GRID_POINTS, CV_64F);
#endif
* */
	loadAnglePoints();
	//initAngleMat_CV2();
	initAngleMat();
    //gridPointsX = anglePointsX;
    //gridPointsY = anglePointsY;
    //maxAngles = {-3.7, 3.7, -3.5, 4.5};
    //VLM = {X_LASER_TO_MEMS, Y_LASER_TO_MEMS, Z_LASER_TO_MEMS};
}


void Laser_pos_control::initAngleMat() {
#ifdef __arm__
	arma::vec X = arma::linspace(0, CAMERA_RESOLUTION - 1, X_ANGLES_GRID_POINTS);
	arma::vec Y = arma::linspace(0, CAMERA_RESOLUTION - 1, Y_ANGLES_GRID_POINTS);
	arma::vec Xi = arma::linspace(0, CAMERA_RESOLUTION - 1, CAMERA_RESOLUTION);
	arma::vec Yi = arma::linspace(0, CAMERA_RESOLUTION - 1, CAMERA_RESOLUTION);
	arma::mat::fixed<CAMERA_RESOLUTION, CAMERA_RESOLUTION>  ZiX;
	arma::mat::fixed<CAMERA_RESOLUTION, CAMERA_RESOLUTION>  ZiY;
	
	arma::interp2(X, Y, gridPointsX, Xi, Yi, ZiX);
	arma::interp2(X, Y, gridPointsY, Xi, Yi, ZiY);

	for (int i = 0; i < CAMERA_RESOLUTION; i++) {
		for (int j = 0; j < CAMERA_RESOLUTION; j++) {
            angleMat[i][j][0] = float(ZiX.at(i, j));
			
            angleMat[i][j][1] = float(ZiY.at(i, j));
		}
	}
#endif
}

/*
void Laser_pos_control::initAngleMat_CV2() {
#ifdef __arm__

	cv::Mat dstX, dstY;
	//cv::Mat map_x, map_y;

	dstX.create(CAMERA_RESOLUTION, CAMERA_RESOLUTION, CV_64F);
	dstY.create(CAMERA_RESOLUTION, CAMERA_RESOLUTION, CV_64F);

	resize(gridPointsX, dstX, dstX.size(), 0, 0, cv::INTER_LINEAR);
	resize(gridPointsY, dstY, dstY.size(), 0, 0, cv::INTER_LINEAR);
	
	std::cout << dstX << std::endl;
	std::cout << dstY << std::endl;

	for (int i = 0; i < CAMERA_RESOLUTION; i++) {
		for (int j = 0; j < CAMERA_RESOLUTION; j++) {
			angleMat[i][j][0] = (short)(dstX.at<double>(i, j) * 1000);
			angleMat[i][j][1] = (short)(dstY.at<double>(i, j) * 1000);
		}
	}
#endif // __arm__
}
*/

/*
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
*/

float* Laser_pos_control::getAngles(int xCoord, int yCoord) {
	static float XYAngles[2];

    XYAngles[0] = angleMat[yCoord][xCoord][0];
    XYAngles[1] = angleMat[yCoord][xCoord][1];

	return XYAngles;
}

/*
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
*/

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
/*
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
*/

void Laser_pos_control::draw_rectangle(int time_delay) {
#ifdef __arm__
	//float *angles;
	for (int i = 0; i < RECTANGLE_SEQUENCE_LENGTH; i++) {
		send_pos(rectangle_LUT[i][0], rectangle_LUT[i][1]);
		//angles = getAngles(rectangle_LUT[i][0], rectangle_LUT[i][1]);
		//mems.send_angles(angles[0], angles[1]);
		delay(time_delay);
		//std::cout<<angles[0]<<angles[1]<<std::endl;
	}
#endif
}

void Laser_pos_control::draw_closing_rectangle(int time_delay) {
#ifdef __arm__
	//float *angles;
	for (int i = 0; i < CLOSING_RECTANGLE_LENGTH; i++) {
		send_pos(closingRect_LUT[i][0], closingRect_LUT[i][1]);
		//angles = getAngles(closingRect_LUT[i][0], closingRect_LUT[i][1]);
		//mems.send_angle_x(*angles++);
		//cout << *angles << endl;
		//mems.send_angle_y(*angles);
		delay(time_delay);
	}
#endif
}

void Laser_pos_control::draw_spiral(int time_delay) {
#ifdef __arm__
	//float *angles;
	for (int i = 0; i < SPIRAL_RESOLUTION; i++) {
		send_pos(spiral_LUT[i][0], spiral_LUT[i][1]);
		//angles = getAngles(spiral_LUT[i][0], spiral_LUT[i][1]);
		//mems.send_angle_x(*angles++);
		//mems.send_angle_y(*angles);
		delay(time_delay);
	}
#endif
}

void Laser_pos_control::draw_infinity(int time_delay) {
#ifdef __arm__
	//float *angles;
	for (int i = 0; i < INFINITY_SEQUENCE_LENGTH; i++) {
		send_pos(infinity_LUT[i][0], infinity_LUT[i][1]);
		//angles = getAngles(infinity_LUT[i][0], infinity_LUT[i][1]);
		//mems.send_angle_x(*angles++);
		//cout << *angles << endl;
		//mems.send_angle_y(*angles);
		delay(time_delay);
	}
#endif
}

void Laser_pos_control::draw_circular_loop(int time_delay){
#ifdef __arm__
	//float *angles;
	for (int i = 0; i < CIRCULAR_LOOP_SEQUENCE_LENGTH; i++) {
		send_pos(circularLoop_LUT[i][0], circularLoop_LUT[i][1]);
		//angles = getAngles(circularLoop_LUT[i][0], circularLoop_LUT[i][1]);
		//mems.send_angle_x(*angles++);
		//cout << *angles << endl;
		//mems.send_angle_y(*angles);
		delay(time_delay);
	}
#endif
}

void Laser_pos_control::manual_mode() {
#ifdef __arm__
    const float delta_angle = 0.02;
    const int wait_delay = 100;
    float momentum = delta_angle;
    int axis = 0;
    float angle_x = mems.get_angle_x();
    float angle_y = mems.get_angle_y();

    while(button4.scan_button() != PRESSED) {
        if(button3.scan_button() == PRESSED) { // Change of axis
            axis ^= 1;
        }
        if(axis) {
            if(button1.scan_button() == HELD_DOWN || button1.scan_button() == PRESSED) {
				// Increase x angle
				angle_x += momentum;
                angle_x = mems.send_angle_x(angle_x);
		//cout << "X : " << angle_x << endl;
                momentum += delta_angle;
            } else if(button2.scan_button() == HELD_DOWN || button2.scan_button() == PRESSED) {
				// Decrease x angle
				angle_x -= momentum;
                angle_x = mems.send_angle_x(angle_x);
		//cout << "X : " << angle_x << endl;
                momentum += delta_angle;
            } else { // reset momentum
                momentum = delta_angle;
            }

        } else {
            if(button1.scan_button() == HELD_DOWN || button1.scan_button() == PRESSED) {
				// Increase y angle
				angle_y += momentum;
                angle_y = mems.send_angle_y(angle_y);
		//cout << "Y : "  << angle_y << endl;
                momentum += delta_angle;
			} else if(button2.scan_button() == HELD_DOWN || button2.scan_button() == PRESSED) {
				// Decrease y angle
				angle_y -= momentum;
                angle_y = mems.send_angle_y(angle_y);
		//cout << "Y : "  << angle_y << endl;
                momentum += delta_angle;
            } else { // reset momentum
                momentum = delta_angle;
            }

        }
        delay(wait_delay);
    }
#endif
}

/*void Laser_pos_control::set_max_angles() { // TODO: get angle_x and angle_y
    int corner = 0;

    while(corner <= 1) {
	manual_mode();
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
}*/

void Laser_pos_control::send_pos(int x, int y){
#ifdef __arm__
	float *XYAngles;
	//int x_inverted = CAMERA_RESOLUTION - x; // TODO: verify angleMat gen.
	//int y_inverted = CAMERA_RESOLUTION - y; // TODO: verify angleMat gen.
	XYAngles = getAngles(x, y);
	mems.send_angles(XYAngles[0], XYAngles[1]);
#endif
}


void Laser_pos_control::keyboard_manual_mode() {
#ifdef __arm__
    const float delta_angle = 0.01;
    const int wait_delay = 30;
    float momentum = 0;//delta_angle;
    float angle_x = mems.get_angle_x();
    float angle_y = mems.get_angle_y();
    char key = 0;
    char last_key = 0;

    while(key != KB_Q && key != KB_SPACE) {
		//cin >> key; //TODO: replace by getchar()
		key = getch();
		//cout << "key entered : "  << key << endl;
		switch(key) {//TODO: add print on enter
			case KB_W : //KB_UP :
				if(last_key != key) {
					momentum = delta_angle;
				}
				last_key = key;
				// Increase x angle
				angle_x += momentum;
				angle_x = mems.send_angle_x(angle_x);
				//cout << "X : " << angle_x << endl;
				momentum += delta_angle;
				break;
			case KB_S :  //KB_DOWN :
				if(last_key != key) {
					momentum = delta_angle;
				}
				last_key = key;
				// Decrease x angle
				angle_x -= momentum;
				angle_x = mems.send_angle_x(angle_x);
				//cout << "X : " << angle_x << endl;
				momentum += delta_angle;
				break;
			case KB_D : //KB_RIGHT:
				if(last_key != key) {
					momentum = delta_angle;
				}
				last_key = key;
				// Increase y angle
				angle_y += momentum;
				angle_y = mems.send_angle_y(angle_y);
				//cout << "Y : "  << angle_y << endl;
				momentum += delta_angle;
				break;
			case KB_A : // KB_LEFT :
				if(last_key != key) {
					momentum = delta_angle;
				}
				last_key = key;
				// Decrease y angle
				angle_y -= momentum;
				angle_y = mems.send_angle_y(angle_y);
				//cout << "Y : "  << angle_y << endl;
				momentum += delta_angle;
				break;

			default :
				break;
		}
        delay(wait_delay);
    }
#endif
}
/*
void Laser_pos_control::export2Header(const char *fileName, mat gridPointsX, mat gridPointsY) {
	ofstream myfile;
	myfile.open(fileName);
	myfile << "#pragma once" << endl;
	myfile << "#include <armadillo>" << endl;
	myfile << "arma::mat anglePointsX= {" << endl;
	for (int yIndex = 0; yIndex < Y_ANGLES_GRID_POINTS; yIndex ++) {
		myfile << "\t{";
		for (int xIndex = 0; xIndex < X_ANGLES_GRID_POINTS; xIndex++) {
			myfile << gridPointsX(yIndex, xIndex);
			if(xIndex != X_ANGLES_GRID_POINTS -1){
				myfile << ", ";
			}
		}
		if (yIndex == Y_ANGLES_GRID_POINTS-1) {
			myfile << "}";
		}
		else {
			myfile << "}," << endl;;
		}
    }
	myfile  << endl << "};" << endl << endl;
	myfile << "arma::mat anglePointsY= {" << endl;
	for (int yIndex = 0; yIndex < Y_ANGLES_GRID_POINTS; yIndex ++) {
		myfile << "\t{";
		for (int xIndex = 0; xIndex < X_ANGLES_GRID_POINTS; xIndex++) {
			myfile << gridPointsY(yIndex, xIndex);
			if(xIndex != X_ANGLES_GRID_POINTS -1){
				myfile << ", ";
			}
		}
		if (yIndex == Y_ANGLES_GRID_POINTS-1) {
			myfile << "}";
		}
		else {
			myfile << "}," << endl;;
		}

    }
	myfile << endl << "};" << endl;
	myfile.close();

}
*/

void Laser_pos_control::calibrateGrid() {
#ifdef __arm__

    for (int yIndex = 0; yIndex < Y_ANGLES_GRID_POINTS; yIndex ++) {
		for (int xIndex = 0; xIndex < X_ANGLES_GRID_POINTS; xIndex++) {
			keyboard_manual_mode();
			mems.print_angles() ;
			
			gridPointsX.at(yIndex, xIndex) = (double) mems.get_angle_x();
			gridPointsY.at(yIndex, xIndex) = (double) mems.get_angle_y();

			//gridPointsX.at<double>(yIndex, xIndex) = (double) mems.get_angle_x();
			//gridPointsY.at<double>(yIndex, xIndex) = (double) mems.get_angle_y();
		}
    }
    cout << "Grid points X" << endl;
    gridPointsX.print();
	//cout << gridPointsX << endl;
    cout << "Grid points Y" << endl;
    gridPointsY.print();
	//cout << gridPointsY << endl;

	saveAnglePoints();
   // export2Header(ANGLE_POINTS_FILENAME_H, gridPointsX, gridPointsY);
#endif
}

void Laser_pos_control::saveAnglePoints() {
#ifdef __arm__
	std::ofstream myfile;
	myfile.open(ANGLE_POINTS_FILENAME_TXT);

	for (int i = 0; i < Y_ANGLES_GRID_POINTS; i++) {
		for (int j = 0; j < X_ANGLES_GRID_POINTS; j++) {
			myfile << gridPointsX.at(i, j);
			//myfile << gridPointsX.at<double>(i, j);
			if (j == X_ANGLES_GRID_POINTS - 1) {
				myfile << endl;
			}
			else {
				myfile << " ";
			}
		}
	}

	for (int i = 0; i < Y_ANGLES_GRID_POINTS; i++) {
		for (int j = 0; j < X_ANGLES_GRID_POINTS; j++) {
			myfile << gridPointsY.at(i, j);
			//myfile << gridPointsY.at<double>(i, j);
			if (j == X_ANGLES_GRID_POINTS- 1) {
				myfile << endl;
			}
			else {
				myfile << " ";
			}
		}
	}
	myfile.close();
#endif
}

void Laser_pos_control::loadAnglePoints() {
#ifdef __arm__
	std::ifstream myfile;
	myfile.open(ANGLE_POINTS_FILENAME_TXT);

	for (int i = 0; i < Y_ANGLES_GRID_POINTS; i++) {
		for (int j = 0; j < X_ANGLES_GRID_POINTS; j++) {
			myfile >> gridPointsX.at(i, j);
			//myfile >> gridPointsX.at<double>(i, j);
		}
	}

	for (int i = 0; i < Y_ANGLES_GRID_POINTS; i++) {
		for (int j = 0; j < X_ANGLES_GRID_POINTS; j++) {
			myfile >> gridPointsY.at(i, j);
			//myfile >> gridPointsY.at<double>(i, j);
		}
	}
	myfile.close();
#endif
}
