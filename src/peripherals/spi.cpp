/*
 *  @file spi.cpp
 *  @author Patrice Buteau
 *  @version 26 septembre 2019
 *
 *
 * NOTE : Before you can use SPI interface, you may need to use
 * the gpio utility to load the SPI drivers into the
 * kernel: gpio load spi
 */

#include <wiringPiSPI.h>

#include "spi.h"


Spi::Spi(int chan, int speed, int mode) {
    channel = chan;
    wiringPiSPISetupMode(channel, speed, mode) ;

}


void Spi::send(unsigned char* data, int len) {

    wiringPiSPIDataRW(channel, data, len); // CS?

}
