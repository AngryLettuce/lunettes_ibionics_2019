/*
 *  @file i2c.h
 *	@brief Everything related to I2C.
 *  @author Patrice Buteau
 *  @version 26 septembre 2019
 */

#ifndef SRC_PERIPHERALS_I2C_H_
#define SRC_PERIPHERALS_I2C_H_


class I2c {
public:
	/**
	 * @brief I2c constructor for each device on the bus.
	 * @param port_name i2c port name.
	 * @param address i2c device address.
	 */
	I2c(const char *port_name, unsigned char address);
	/**
	 * @brief Sends out data.
	 * @param data Data to be sent.
	 * @param len Number of bytes to be sent.
	 */
	void send(unsigned char *data, unsigned int len);
private:
	/**
	 * @brief file descriptor of a specific device on the i2c bus.
	 */
	int fd;
};


#endif /* SRC_PERIPHERALS_I2C_H_ */
