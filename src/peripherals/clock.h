/*
 *  @file clock.h
 *	@brief Everything related to clocks.
 *  @author Patrice Buteau
 *  @version 14 octobre 2019
 */

#ifndef SRC_PERIPHERALS_CLOCK_H_
#define SRC_PERIPHERALS_CLOCK_H_


class Clock {
public:
	/**
	 * @brief Clock constructor.
	 * @param pin pin number.
	 */
	Clock(int pin);
	/**
	 * @brief Set the clock frequency.
	 */
    void set_clock(int freq);
private:
	int pin_number;
};


#endif /* SRC_PERIPHERALS_CLOCK_H_ */
