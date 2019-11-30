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
 #ifdef __arm__
#include <wiringPi.h>
#include <wiringPiSPI.h>
#endif

#include "spi.h"

#define GPIO44_SPI2_CS   44
#define GPIO40_SPI2_MISO 40
#define GPIO38_SPI2_MOSI 38
#define GPIO39_SPI2_SCK    39

Spi::Spi(int chan, int speed, int mode) :
    //sck(GPIO39_SPI2_SCK),
    //mosi(GPIO38_SPI2_MOSI),
    //miso(GPIO40_SPI2_MISO),
	cs(GPIO44_SPI2_CS)  {
#ifdef __arm__
    channel = chan;
    
    if(chan == 0) {
        wiringPiSPISetupMode(channel, speed, mode);
    }
    
    cs.write(HIGH);
#endif
}


void Spi::send(unsigned char* data, int len) {
    if(channel == 0) {
    #ifdef __arm__
        cs.write(LOW);
        delayMicroseconds(1);
        wiringPiSPIDataRW(channel, data, len);
        delayMicroseconds(1);
        cs.write(HIGH);
        delayMicroseconds(1);
    #endif
        
    } /*else {
        cs.write(LOW);
        delayMicroseconds(4);
        for(int byte=0; byte < len; byte++) {
            //std::cout << (int)data[byte] << std::endl;
            for(int bit=0; bit < 8; bit++) {
                sck.write(HIGH);
                mosi.write((data[byte] & 0x80) >> 7);
                data[byte] <<= 1;
                delayMicroseconds(4);
                sck.write(LOW);
                //in = (in << 1) | digitalRead(GPIO40_SPI2_MISO);
                delayMicroseconds(4);
            }

        }
        delayMicroseconds(4);
        cs.write(HIGH);
    }*/
}
