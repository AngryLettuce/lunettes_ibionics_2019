/*
 *  @file ir_led.h
 *	@brief Everything related to the infrared LEDs.
 *  @author Patrice Buteau
 *  @version 14 octobre 2019
 */

#ifndef SRC_MODULES_IR_LED_H_
#define SRC_MODULES_IR_LED_H_

#include "../peripherals/digital_out.h"
#include "digital_pot.h"


class Ir_led {
public:
    Digital_pot digital_pot_ir_led;
	/**
	 * @brief Infrared LED constructor.
	 */
	Ir_led();
    /**
	 * @brief Open IR LED.
	 */
	void on();
	/**
	 * @brief Close IR LED.
	 */
	void off();
private:
	Digital_out enable;
};


#endif /* SRC_MODULES_IR_LED_H_ */
