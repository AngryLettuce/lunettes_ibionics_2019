/*
 *  @file main.cpp
 *  @author Patrice Buteau
 *  @version 19 septembre 2019
 */

#include <iostream>

#include <wiringPi.h>

#include "modules/laser_pos_control.h"

using namespace std;

/*
 *  @brief Main function.
 */
int main() {
    wiringPiSetupGpio(); // TODO : Should be done once in gpio.h
    Laser_pos_control laser_pos_control;
    //laser_pos_control.set_max_angles();
    short angleMat[CAMERA_RESOLUTION][CAMERA_RESOLUTION][2];
	laser_pos_control.recalculateAnglesMat(angleMat);
    //cout << "Initializing laser..." << endl;
    //cout << "Laser initialized!" << endl;

   //cout << "Initializing Mems..." << endl;
    //cout << "Mems initialized!" << endl;

    //for(int i=10;i<50; i++) { // number of pupil positions
        //float angle_x = ;
        //float angle_y = ;
        //send_angle_x(angle_x);
        //send_angle_y(angle_y);
        //laser.digital_pot_U15.set_resistance_0_50k(2.5);
        //digitalWrite(37, LOW);
        //delay(1000);
        //digitalWrite(37, HIGH);
        //laser.digital_pot_U15.set_resistance_0_50k(50);
       // delay(1000);
    //}
    //mems.stop();
    return 0;
}
