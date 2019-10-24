/*
 *  @file laser_pos_control.h
 *	@brief Everything related to laser position control.
 *  @author Patrice Buteau et Guillaume Roux
 *  @version 15 octobre 2019
 */

#ifndef SRC_MODULES_LASER_POS_CONTROL_H_
#define SRC_MODULES_LASER_POS_CONTROL_H_

#include <armadillo>

#include "mems.h"
#include "laser.h"
#include "button.h"

#define CAMERA_RESOLUTION 200

using namespace arma;

class Laser_pos_control {
public:
    Laser laser;
    Mems mems;
	/**
	 * @brief Laser_pos_control constructor.
	 */
	Laser_pos_control();

	void recalculateAnglesMat(short angleMat[][CAMERA_RESOLUTION][2]);
	/**
	 * @brief send the rectangle sequence to the mems mirror.
	 * @param time_delay delay between mirror commands.
	 */
	void draw_rectangle(int time_delay);
	/**
	 * @brief send the closing rectangle sequence to the mems mirror.
	 * @param time_delay delay between mirror commands.
	 */
	void draw_Closingrectangle(int time_delay);
	/**
	 * @brief send the spiral sequence to the mems mirror.
	 * @param time_delay delay between mirror commands.
	 */
	void draw_spiral(int time_delay);
	/**
	 * @brief send the infinity symbol sequence to the mems mirror.
	 * @param time_delay delay between mirror commands.
	 */
	void draw_infinity(int time_delay);

    void draw_circluarLoop(int time_delay);

    void set_max_angles();

    float* manual_mode();
private:
    Button button1;
    Button button2;
    Button button3;
    Button button4;
    rowvec maxAngles;

    void findWallCorners(mat &wallCorners);

    void genPixMat(mat wallCorners, mat &pixMat);

    double *getAngle(int xCoord, int yCoord);

    void genXYZ_Matrix(mat &XYZ_Matrix);

    void genAnglesTable(mat pixMat, mat XYZ_Matrix, short angleMat[][CAMERA_RESOLUTION][2]);

    //void findAngles(double x, double y, mat XYZ_Matrix, double *angles);

	void angle2XY(float aX, float aY, float &x, float &y);

	double deg2rad(float angle);

    int calcArraySize();
};



#endif /* SRC_MODULES_LASER_POS_CONTROL_H_ */
