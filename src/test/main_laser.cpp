/*
 * main.cpp
 *
 *  Created on: 26 septembre 2019
 *      Author: Patrice B.
 */

#include "wiringPi.h"
#include "wiringPiI2C.h"

#define GPIO32_LASER_EN_DRV     32
#define GPIO30_LASER_ENCAL      30
#define GPIO38_LASER_EN_CH_LOW  38
#define GPIO37_LASER_EN_CH_MID  37
#define GPIO33_LASER_EN_CH_HIGH 33
#define GPIO31_LASER_REGEN      31

unsigned char SendBuffer[2];
const int WRITE_RESISTANCE_COMMAND = 0x04;


/*Temporary code for laser driver*/
typedef enum {
	DIGITAL_POTENTIOMETER_address_U13 = 47,
	DIGITAL_POTENTIOMETER_address_U14 = 46,
	DIGITAL_POTENTIOMETER_address_U15 = 44
} DIGITAL_POTENTIOMETER_Address;


int DIGITAL_POTENTIOMETER_set_resistance_0_50k(float resistance_kilohoms, DIGITAL_POTENTIOMETER_Address address){
	// Activate
	SendBuffer[0] = 0x1C;
	SendBuffer[1] = 0x03;
	int fd = wiringPiI2CSetupInterface("/dev/i2c-0",address);
	
	wiringPiI2CWrite(fd, SendBuffer[0]);
	wiringPiI2CWrite(fd, SendBuffer[1]);

	unsigned short pot_value_10bits = resistance_kilohoms*20.48;

	if (pot_value_10bits > 1024){
		return -1;
	}
	if (pot_value_10bits == 1024){
		pot_value_10bits = 1023;
	}

	SendBuffer[0] = WRITE_RESISTANCE_COMMAND + ((pot_value_10bits>>8)& 0x3);
	SendBuffer[1] = pot_value_10bits & 0xFF;
	
	wiringPiI2CWrite(fd, SendBuffer[0]);
	wiringPiI2CWrite(fd, SendBuffer[1]);

	return 0;
}
/*------------------------------------------------*/

void off() {
	digitalWrite(GPIO32_LASER_EN_DRV, LOW);
}


void on() {
	digitalWrite(GPIO32_LASER_EN_DRV, HIGH);
}


int main() {
	// Configure LASER GPIOs
	wiringPiSetupGpio();
	/*pinMode(28, OUTPUT);
	while(1) {
	digitalWrite(28, HIGH);
	delay(500);
	digitalWrite(28, LOW);
	delay(500);
	}*/
	
	pinMode(GPIO32_LASER_EN_DRV, OUTPUT);
	pinMode(GPIO30_LASER_ENCAL, OUTPUT);
	pinMode(GPIO31_LASER_REGEN, OUTPUT);
	pinMode(GPIO38_LASER_EN_CH_LOW, OUTPUT);
	pinMode(GPIO37_LASER_EN_CH_MID, OUTPUT);
	pinMode(GPIO33_LASER_EN_CH_HIGH, OUTPUT);
	
	digitalWrite(GPIO31_LASER_REGEN, HIGH);
	digitalWrite(GPIO32_LASER_EN_DRV, LOW);
	digitalWrite(GPIO38_LASER_EN_CH_LOW, LOW);
	digitalWrite(GPIO37_LASER_EN_CH_MID, LOW);
	digitalWrite(GPIO33_LASER_EN_CH_HIGH, LOW);

	digitalWrite(GPIO30_LASER_ENCAL, HIGH);
	delay(1000);
	digitalWrite(GPIO32_LASER_EN_DRV, HIGH);
	delay(1000);
	// Set intensity here
	digitalWrite(GPIO38_LASER_EN_CH_LOW, HIGH);

	// Set laser power
	DIGITAL_POTENTIOMETER_set_resistance_0_50k(10,
			DIGITAL_POTENTIOMETER_address_U15); //LOW
	DIGITAL_POTENTIOMETER_set_resistance_0_50k(20,
			DIGITAL_POTENTIOMETER_address_U14); //MID
	DIGITAL_POTENTIOMETER_set_resistance_0_50k(50,
			DIGITAL_POTENTIOMETER_address_U13); //HIGH
	
	return 0;
}

