/*
 *  @file digital_out.h
 *	@brief Everything related to digital outputs.
 *  @author Patrice Buteau
 *  @version 14 octobre 2019
 */

#ifndef SRC_PERIPHERALS_DIGITAL_OUT_H_
#define SRC_PERIPHERALS_DIGITAL_OUT_H_


class Digital_out {
public:
	/**
	 * @brief Digital output constructor.
	 * @param pin pin number.
	 */
	Digital_out(int pin);
	/**
	 * @brief Set output level.
	 * @param value output level.
	 */
	void write(int value);
private:
	int pin_number;
};


#endif /* SRC_PERIPHERALS_DIGITAL_OUT_H_ */
