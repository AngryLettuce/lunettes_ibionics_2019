/*
 *  @file clock.cpp
 *  @author Patrice Buteau
 *  @version 14 octobre 2019
 */

#include <wiringPi.h>

#include "clock.h"


Clock::Clock(int pin) {
    pinMode(pin, GPIO_CLOCK);
    pin_number = pin;
}

void Clock::set_clock(int freq) {
    gpioClockSet(pin_number, freq);
}