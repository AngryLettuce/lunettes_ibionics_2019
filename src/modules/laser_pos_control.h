/*
 *  @file laser_pos_control.h
 *	@brief Everything related to laser position control.
 *  @author Patrice Buteau et Guillaume Roux
 *  @version 15 octobre 2019
 */

#ifndef SRC_MODULES_LASER_POS_CONTROL_H_
#define SRC_MODULES_LASER_POS_CONTROL_H_

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "mems.h"
#include "laser.h"
#include "button.h"

#define CAMERA_RESOLUTION 200

#define X_ANGLES_GRID_POINTS 5
#define Y_ANGLES_GRID_POINTS 4

#ifdef __arm__
//using namespace arma;
#endif

class Laser_pos_control {
public:

    Laser laser;
    Mems mems;
#ifdef __arm__
	//mat::fixed<Y_ANGLES_GRID_POINTS, X_ANGLES_GRID_POINTS> gridPointsX;
	//mat::fixed<Y_ANGLES_GRID_POINTS, X_ANGLES_GRID_POINTS> gridPointsY;
	cv::Mat gridPointsX, gridPointsY;
#endif
	/**
	 * @brief Laser_pos_control constructor.
	 */
	Laser_pos_control();
	/**
	 * @brief Generate the angleMat with a bilinear interpolation with armadillo
	 */
	void initAngleMat();
	/**
	 * @brief Generate the angleMat with a bilinear interpolation with opencv
	*/
	void initAngleMat_CV2();
	/**
	 * @brief Recalculate the angleMat with the mathematic model.
	 */
	//void recalculateAnglesMat();
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
	/**
	 * @brief send the circular loop symbol sequence to the mems mirror.
	 * @param time_delay delay between mirror commands.
	 */
	void draw_circluarLoop(int time_delay);
	/**
	 * @brief Set the maximum angles in x and y using the manual mode
	 */
    // void set_max_angles();
	/**
	 * @brief Control manually the MEMS mirror.
	 * @return last x and y angle sent
	 */
	void manual_mode();
    	/**
	 * @brief Control manually the MEMS mirror with the keyboard.
	 * @return last x and y angle sent
	 */
	void keyboard_manual_mode();
	/**
	 * @brief Send the x and y coordinates to the MEMS
	 */
	void send_pos(int x, int y);
    	/**
	 * @brief write the grid points in X and Y to a header file
	 */
    #ifdef __arm__
	//void export2Header(const char *fileName, mat gridPointsX, mat gridPointsY);
	#endif
	/**
	 * @brief fille gridPointX and gridPointY with the keyboard manual mode
	 */
	void calibrateGrid();
	/**
	* @brief save the gridPoints of the calibration sequence to a txt file
	*/
	void saveAnglePoints();
	/**
	* @brief load and populate the gridPoints from a txt file
	*/
	void loadAnglePoints();
private:
    Button button1;
    Button button2;
    Button button3;
    Button button4;
   // mat gridPointsX;
   // mat gridPointsY;
       
    //rowvec maxAngles; // {minX, maxX, minY, maxY}
    //rowvec VLM;
    short angleMat[CAMERA_RESOLUTION][CAMERA_RESOLUTION][2];

    //void findWallCorners(mat &wallCorners);

    //void genPixMat(mat wallCorners, mat &pixMat);

    float *getAngles(int xCoord, int yCoord);

    //void genXYZ_Matrix(mat &XYZ_Matrix);

    //void genAnglesTable(mat pixMat, mat XYZ_Matrix, short angleMat[][CAMERA_RESOLUTION][2]);

    //void findAngles(double x, double y, mat XYZ_Matrix, double *angles);

    //void angle2XY(float aX, float aY, float &x, float &y);

    //double deg2rad(float angle);

    //int calcArraySize();
};

#endif /* SRC_MODULES_LASER_POS_CONTROL_H_ */
