/*
 *  @file digital_in.cpp
 *  @author Patrice Buteau
 *  @version 11 octobre 2019
 */

#include <wiringPi.h>

#include "digital_in.h"


Digital_in::Digital_in(int pin) {
    pinMode(pin, INPUT);
    pin_number = pin;
}

int Digital_in::read() {
    digitalRead(pin_number);
}

void Digital_in::set_pull(int pull) {
    pullUpDnControl(pin_number, pull);
}