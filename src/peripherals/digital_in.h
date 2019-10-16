/*
 *  @file digital_in.h
 *	@brief Everything related to digital inputs.
 *  @author Patrice Buteau
 *  @version 11 octobre 2019
 */

#ifndef SRC_PERIPHERALS_DIGITAL_IN_H_
#define SRC_PERIPHERALS_DIGITAL_IN_H_


class Digital_in {
public:
	/**
	 * @brief Digital input constructor.
	 * @param pin pin number.
	 */
	Digital_in(int pin);
	/**
	 * @brief Read the value of a given pin.
	 * @return HIGH or LOW
	 */
    int read();
	/**
	 * @brief Set the input pin mode.
	 * @param pull PUD_OFF, PUD_DOWN, PUD_UP.
	 */
    void set_pull(int pull);
private:
	int pin_number;
};


#endif /* SRC_PERIPHERALS_DIGITAL_IN_H_ */
