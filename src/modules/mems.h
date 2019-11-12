/*
 *  @file mems.h
 *	@brief Everything related to MEMS mirror.
 *  @author Patrice Buteau
 *  @version 28 juin 2019
 */

#ifndef SRC_MODULES_MEMS_H_
#define SRC_MODULES_MEMS_H_



#include "../peripherals/spi.h"
#include "../peripherals/digital_out.h"
//#include "../peripherals/clock.h"


class Mems {
public:
	/**
	 * @brief Mems constructor.
	 */
	Mems();
	/**
	 * @brief Send an angle command for the x axis.
	 * @param angle x angle (deg).
	 */
	void send_angle_x(float angle);
	/**
	 * @brief Send an angle command for the y axis.
	 * @param angle y angle (deg).
	 */
	void send_angle_y(float angle);
	/**
	 * @brief Send both angles.
	 * @param angle_x x angle (deg).
	 * @param angle_y y angle (deg).
	 */
	void send_angles(float angle_x, float angle_y);
	/**
	 * @brief Disable the mems mirror and put it back at zero in x and y.
	 */
	void stop();
	/**
	 * @brief Saturate the angle inside the safe range.
	 * @param angle angle (deg).
	 * @return the saturated angle.
	 */
	float saturate_angle(float angle);
private:
	Spi spi;
	Digital_out enable;
	//Digital_out cs;
	//Clock clock;

	/**
	 * @brief Send data to the DAC.
	 * @param data data to be sent
	 */
	void send_data(unsigned int data);
	/**
	 * @brief Send an angle command for the x axis directly with the voltage diff.
	 * @param voltage_diff x voltage difference (V).
	 */
	void send_voltage_diff_x(float voltage_diff);
	/**
	 * @brief Send an angle command for the y axis directly with the voltage diff.
	 * @param voltage_diff y voltage difference (V).
	 */
	void send_voltage_diff_y(float voltage_diff);
	/**
	 * @brief Saturate the voltage diff. inside the safe range.
	 * @param voltage_diff voltage difference (V).
	 * @return the saturated voltage diff.
	 */
	float saturate_voltage_diff(float voltage_diff);
	/**
	 * @brief Convert an angle to a voltage diff.
	 * @param angle the angle to convert (deg).
	 * @return the corresponding voltage diff.
	 */
	float angle_to_voltage_diff(float angle);
	/**
	 * @brief Convert a voltage diff to an angle.
	 * @param voltage_diff the voltage diff to convert (V).
	 * @return the corresponding angle.
	 */
	float voltage_diff_to_angle(float voltage_diff);
	/**
	 * @brief Itinialize the DAC.
	 */
	void init_DAC();
};



#endif /* SRC_MODULES_MEMS_H_ */
