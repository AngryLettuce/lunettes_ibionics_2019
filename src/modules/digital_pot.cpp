/*
 *  @file digital_pot.cpp
 *  @author Patrice Buteau
 *  @version 3 octobre 2019
 *
 *
 * NOTE : Before you can use I2C interface, you may need to use
 * the gpio utility to load the SPI drivers into the
 * kernel: gpio load i2c
 */

#include "digital_pot.h"

#define WRITE_RESISTANCE_COMMAND 0x04


Digital_pot::Digital_pot(const char* port_name, unsigned char address) :
	i2c(port_name, address) {

}


int Digital_pot::set_resistance_0_50k(float resistance_kilohoms) {
	unsigned char SendBuffer[2];
	// Activate
	SendBuffer[0] = 0x1C;
	SendBuffer[1] = 0x03;

	i2c.send(SendBuffer, 2);

	unsigned short pot_value_10bits = resistance_kilohoms*20.48;

	if (pot_value_10bits > 1024){
		return -1;
	}
	if (pot_value_10bits == 1024){
		pot_value_10bits = 1023;
	}

	SendBuffer[0] = WRITE_RESISTANCE_COMMAND + ((pot_value_10bits>>8)& 0x3);
	SendBuffer[1] = pot_value_10bits & 0xFF;

	i2c.send(SendBuffer, 2);
	return 0;
}





