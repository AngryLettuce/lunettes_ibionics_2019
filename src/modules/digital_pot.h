/*
 *  @file digital_pot.h
 *	@brief Everything related to digital potentiometers.
 *  @author Patrice Buteau
 *  @version 3 octobre 2019
 */

#ifndef SRC_MODULES_DGITAL_POT_H_
#define SRC_MODULES_DIGITAL_POT_H_

#include "../peripherals/i2c.h"

// LASER INTENSITY
#define DIGITAL_POTENTIOMETER_ADDRESS_U13 47
#define DIGITAL_POTENTIOMETER_ADDRESS_U14 46
#define DIGITAL_POTENTIOMETER_ADDRESS_U15 44

// IR LED INTENSITY
#define DIGITAL_POTENTIOMETER_ADDRESS_IR  46  // 0b0101110

class Digital_pot {
public:
	/**
	 * @brief Digital potentiometer constructor.
	 * @param port_name i2c port name.
	 * @param address i2c device address.
	 */
	Digital_pot(const char* port_name, unsigned char address);
	/**
	 * @brief Set the potentiometer value.
	 * @param resistance_kilohoms Resistance value (kilohoms).
	 * @return 0 if ok, -1 if not
	 */
	int set_resistance_0_50k(float resistance_kilohoms);
private:
	I2c i2c;
};


#endif /* SRC_MODULES_DIGITAL_POT_H_ */
