/*
 *  @file ir_led.cpp
 *  @author Patrice Buteau
 *  @version 14 octobre 2019
 */
#ifdef __arm__
#include <wiringPi.h>
#endif
#include "ir_led.h"

#define GPIO24_IR_LED_ENABLE   24


Ir_led::Ir_led() :
 	digital_pot_ir_led("/dev/i2c-0", DIGITAL_POTENTIOMETER_ADDRESS_IR),
    enable(GPIO24_IR_LED_ENABLE) {
    #ifdef __arm__
    enable.write(HIGH);
    #endif
}


void Ir_led::on() {
    #ifdef __arm__
	enable.write(HIGH);
    #endif
}


void Ir_led::off() {
    #ifdef __arm__
	enable.write(LOW);
#endif
}




