/*
 *  @file spi.h
 *	@brief Everything related to SPI.
 *  @author Patrice Buteau
 *  @version 26 septembre 2019
 */

#ifndef SRC_PERIPHERALS_SPI_H_
#define SRC_PERIPHERALS_SPI_H_

#include "digital_out.h"
#include "digital_in.h"

class Spi {
public:
	/**
	 * @brief Spi constructor
	 * @param chan spi port number.
	 * @param speed baudrate (Hz).
	 * @param mode spi mode.
	 */
	Spi(int chan, int speed, int mode);
	/**
	 * @brief Sends out data.
	 * @param data Data to be sent.
	 * @param len Number of bytes to be sent.
	 */
	void send(unsigned char* data, int len);
private:
	Digital_out cs;
	//Digital_out sck;
	//Digital_out mosi;
	//Digital_in miso;
	int channel;
};


#endif /* SRC_PERIPHERALS_SPI_H_ */
