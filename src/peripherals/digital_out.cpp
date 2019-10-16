/*
 *  @file digital_out.cpp
 *  @author Patrice Buteau
 *  @version 14 octobre 2019
 */

#include <wiringPi.h>

#include "digital_out.h"


Digital_out::Digital_out(int pin) {
    pinMode(pin, OUTPUT);
    pin_number = pin;
}


void Digital_out::write(int value) {
    digitalWrite(pin_number, value);
}