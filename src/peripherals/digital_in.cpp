/*
 *  @file digital_in.cpp
 *  @author Patrice Buteau
 *  @version 11 octobre 2019
 */
#ifdef __arm__
#include <wiringPi.h>
#endif

#include "digital_in.h"


Digital_in::Digital_in(int pin) {
#ifdef __arm__
    pinMode(pin, INPUT);
    pin_number = pin;
#endif
}

int Digital_in::read() {
#ifdef __arm__
    return digitalRead(pin_number);
#endif
}

void Digital_in::set_pull(int pull) {
#ifdef __arm__
    pullUpDnControl(pin_number, pull);
#endif
}
