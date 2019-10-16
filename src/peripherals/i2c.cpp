/*
 *  @file i2c.cpp
 *  @author Patrice Buteau
 *  @version 26 septembre 2019
 *
 * NOTE : Before you can use I2C interface, you may need to use
 * the gpio utility to load the SPI drivers into the
 * kernel: gpio load i2c
 */

#include <wiringPiI2C.h>

#include "i2c.h"


I2c::I2c(const char *port_name, unsigned char address) {
	fd = wiringPiI2CSetupInterface(port_name, address);
}


void I2c::send(unsigned char *data, unsigned int len) {
    for(int i=0; i<len; i++) {
        wiringPiI2CWrite(fd, data[i]);
    }

}
