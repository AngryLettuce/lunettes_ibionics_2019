/*
 *  @file laser.h
 *	@brief Everything related to the laser.
 *  @author Patrice Buteau
 *  @version 17 septembre 2019
 */

#ifndef SRC_MODULES_LASER_H_
#define SRC_MODULES_LASER_H_

#include "digital_pot.h"
#include "../peripherals/digital_out.h"

class Laser {
public:
	Digital_pot digital_pot_U13;
	Digital_pot digital_pot_U14;
	Digital_pot digital_pot_U15;
	Digital_out enable;
	Digital_out encal;
	Digital_out regen;
	Digital_out low;
	Digital_out mid;
	Digital_out high;
	/**
	 * @brief Laser constructor.
	 */
	Laser();
	/**
	 * @brief Open laser.
	 */
	void on();
	/**
	 * @brief Close laser.
	 */
	void off();
private:

};


#endif /* SRC_MODULES_LASER_H_ */
