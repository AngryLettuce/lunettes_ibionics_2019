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
//#include <iostream>
 
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "spi.h"

#define GPIO44_SPI2_CS   44
#define GPIO40_SPI2_MISO 40
#define GPIO41_SPI2_MOSI 41
#define GPIO_SPI2_SCK    42

Spi::Spi(int chan, int speed, int mode) {
    channel = chan;
    if(chan == 0) {
        wiringPiSPISetupMode(channel, speed, mode);
    } else if(chan == 2) { // using bit banging for spi2 in mode 1
        pinMode(GPIO44_SPI2_CS, OUTPUT);
        pinMode(GPIO_SPI2_SCK, OUTPUT);
        pinMode(GPIO41_SPI2_MOSI, OUTPUT);
        pinMode(GPIO40_SPI2_MISO, INPUT);

        digitalWrite(GPIO44_SPI2_CS, HIGH);
    }
}


void Spi::send(unsigned char* data, int len) {
    if(channel == 0) {
        wiringPiSPIDataRW(channel, data, len);
    } else if(channel == 2) {
        digitalWrite(GPIO44_SPI2_CS, LOW);
        delayMicroseconds(2);
        for(int byte=0; byte < len; byte++) {
            //std::cout << (int)data[byte] << std::endl;
            for(int bit=0; bit < 8; bit++) {
                digitalWrite(GPIO_SPI2_SCK, HIGH);
                digitalWrite(GPIO41_SPI2_MOSI, (data[byte] & 0x80) >> 7);
                data[byte] <<= 1;
                delayMicroseconds(2);
                digitalWrite(GPIO_SPI2_SCK, LOW);
                //in = (in << 1) | digitalRead(GPIO40_SPI2_MISO);
                delayMicroseconds(2);
            }

        }
        delayMicroseconds(2);
        digitalWrite(GPIO44_SPI2_CS, HIGH);
    }
}
