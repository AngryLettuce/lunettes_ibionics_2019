/*
 *  @file main.cpp
 *  @author Patrice Buteau
 *  @version 19 septembre 2019
 */

#include <iostream>

#include <wiringPi.h>

#include "modules/mems.h"
#include "modules/laser.h"
#include "modules/laser_pos_control.h"

using namespace std;

/*
 *  @brief Main function.
 */
int main() {
    wiringPiSetupGpio(); // TODO : Should be done once in gpio.h
    Laser_pos_control laser_pos_control;
    std::cout<< "Initialized" << endl;
    //laser_pos_control.mems.stop();
    laser_pos_control.manual_mode();
    //laser_pos_control.draw_rectangle(400);
    //laser_pos_control.set_max_angles();
    std::cout<< "Max angles set" << endl;
	//laser_pos_control.recalculateAnglesMat();
        
    while(1) {
           // laser_pos_control.mems.send_angle_x(0);
           laser_pos_control.draw_rectangle(1);
           //laser_pos_control.draw_circluarLoop(100);
        delay(1000);
        laser_pos_control.send_pos(100,100);
        delay(2000);
    }
    
    //laser_pos_control.set_max_angles();
    //short angleMat[CAMERA_RESOLUTION][CAMERA_RESOLUTION][2];
	//laser_pos_control.recalculateAnglesMat(angleMat);
    //cout << "Initializing laser..." << endl;
    //Laser laser;
    //cout << "Laser initialized!" << endl;
    //Mems mems;
   //cout << "Initializing Mems..." << endl;
    //cout << "Mems initialized!" << endl;

   // for(int i=1;i<60; i++) { // number of pupil positions
        //float angle_x = ;
        //float angle_y = ;
        //send_angle_x(angle_x);
        //send_angle_y(angle_y);
        //laser.digital_pot_U15.set_resistance_0_50k(2.5);
        //digitalWrite(37, LOW);
        //delay(1000);
        //digitalWrite(37, HIGH);
        //laser.digital_pot_U15.set_resistance_0_50k(50);
     //   delay(1000);
    //}
    //laser.off();
    //mems.stop();
    return 0;
}
