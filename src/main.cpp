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
    //piHiPri(20);
    Laser_pos_control laser_pos_control;
    
    laser_pos_control.send_pos(100,100);
    laser_pos_control.calibrateGrid();
    //laser_pos_control.export2Header("modules/testing.h");
    //laser_pos_control.manual_mode();
    //laser_pos_control.mems.print_angles() ;
    //laser_pos_control.mems.stop();
    //laser_pos_control.laser.off();
    
    //laser_pos_control.draw_rectangle(400);
    //laser_pos_control.set_max_angles();
	//laser_pos_control.recalculateAnglesMat();
        
    //for(int i=0; i<25; i++) {
    while(1){
        //laser_pos_control.keyboard_manual_mode();
        //laser_pos_control.mems.print_angles() ;
        //angles = laser_pos_control.manual_mode();
        //cout << angles[0] << endl;
        //cout << angles[1] << endl;
           // laser_pos_control.mems.send_angle_x(0);
            
           for(int i=0; i<3; i++){
               laser_pos_control.draw_rectangle();
           }
          
           for(int i=0; i<3; i++){
               laser_pos_control.draw_circluarLoop();
           }
           for(int i=0; i<3; i++){
               laser_pos_control.draw_infinity();
           }
           for(int i=0; i<3; i++){
               laser_pos_control.draw_spiral();
           }
           for(int i=0; i<3; i++){
               laser_pos_control.draw_Closingrectangle();
           }
           
           
        //delay(10);
        laser_pos_control.send_pos(100,100);
        //delay(1000);
        delay(2000);
        //laser_pos_control.manual_mode();
    //laser_pos_control.mems.print_angles() ;
}
    
    //laser_pos_control.set_max_angles();
    //short angleMat[CAMERA_RESOLUTION][CAMERA_RESOLUTION][2];
	//laser_pos_control.recalculateAnglesMat(angleMat);


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
    laser_pos_control.laser.off();
    laser_pos_control.mems.stop();
    return 0;
}
