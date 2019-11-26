/*
 *  @file digital_out.cpp
 *  @author Patrice Buteau
 *  @version 14 octobre 2019
 */
#ifdef __arm__
#include <wiringPi.h>
#endif

#include "digital_out.h"


Digital_out::Digital_out(int pin) {
#ifdef __arm__
    pinMode(pin, OUTPUT);
    pin_number = pin;
#endif
}


void Digital_out::write(int value) {
#ifdef __arm__
    digitalWrite(pin_number, value);
#endif
}
