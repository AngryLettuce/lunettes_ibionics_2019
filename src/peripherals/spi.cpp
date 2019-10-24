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
#include <pigpio.h>

#include "spi.h"

#define GPIO44_SPI2_CS   44
#define GPIO40_SPI2_MISO 40
#define GPIO41_SPI2_MOSI 41
#define GPIO_SPI2_SCK    42

Spi::Spi(int chan, int speed, int mode) {
    channel = chan;
    if(chan == 0) {
        wiringPiSPISetupMode(channel, speed, mode);
    } else if(chan == 2) { // using bit banging for spi2 in
        gpioInitialise(); // TODO: Verify if it interferes with wiringPiSetupGpio()
        bbSPIOpen(GPIO44_SPI2_CS, GPIO40_SPI2_MISO,
                    GPIO41_SPI2_MOSI, GPIO_SPI2_SCK, speed, mode);
    }
}


void Spi::send(unsigned char* data, int len) {
    if(channel == 0) {
        wiringPiSPIDataRW(channel, data, len);
    } else if(channel == 2) {
        char* inBuf;
        bbSPIXfer(GPIO44_SPI2_CS, (char *)data, inBuf, len);
    }
}
