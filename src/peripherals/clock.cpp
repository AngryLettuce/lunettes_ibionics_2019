/*
 *  @file clock.cpp
 *  @author Patrice Buteau
 *  @version 14 octobre 2019
 */
#ifdef __arm__
#include <wiringPi.h>
#endif
#include "clock.h"


Clock::Clock(int pin) {
#ifdef __arm__
    pinMode(pin, GPIO_CLOCK);
    pin_number = pin;
#endif
}

void Clock::set_clock(int freq) {
#ifdef __arm__
    gpioClockSet(pin_number, freq);
#endif
}
