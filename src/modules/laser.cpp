/*
 *  @file laser.cpp
 *  @author Patrice Buteau
 *  @version 17 septembre 2019
 */
#include <wiringPi.h>

#include "laser.h"

#define GPIO32_LASER_EN_DRV     32
#define GPIO30_LASER_ENCAL      30
#define GPIO36_LASER_EN_CH_LOW  36
#define GPIO37_LASER_EN_CH_MID  37
#define GPIO33_LASER_EN_CH_HIGH 33
#define GPIO31_LASER_REGEN      31


Laser::Laser() :
	//digital_pot_U13("/dev/i2c-0", DIGITAL_POTENTIOMETER_ADDRESS_U13),
	//digital_pot_U14("/dev/i2c-0", DIGITAL_POTENTIOMETER_ADDRESS_U14),
	//digital_pot_U15("/dev/i2c-0", DIGITAL_POTENTIOMETER_ADDRESS_U15),
	enable(GPIO32_LASER_EN_DRV),
	encal(GPIO30_LASER_ENCAL),
	regen(GPIO31_LASER_REGEN),
	low(GPIO36_LASER_EN_CH_LOW),
	mid(GPIO37_LASER_EN_CH_MID),
	high(GPIO33_LASER_EN_CH_HIGH)  {
	// Configure LASER GPIOs
	regen.write(HIGH);
	enable.write(LOW);
	low.write(LOW);
	mid.write(LOW);
	high.write(LOW);

	encal.write(HIGH);
	delay(1000);

	enable.write(HIGH);
	delay(1000);
	// Set intensity here
	low.write(HIGH);

	// Set laser power
	//digital_pot_U13.set_resistance_0_50k(50);
	//digital_pot_U14.set_resistance_0_50k(20);
	//digital_pot_U15.set_resistance_0_50k(10);
}


void Laser::off() {
	enable.write(LOW);
}

void Laser::on() {
	enable.write(HIGH);
	///digitalWrite(GPIO32_LASER_EN_DRV, HIGH);
}


