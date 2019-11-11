/*
 *  @file mems.cpp
 *  @author Patrice Buteau
 *  @version 28 juin 2019
 *
 * NOTE : Before you can use SPI interface, you may need to use
 * the gpio utility to load the SPI drivers into the
 * kernel: gpio load spi
 */

#include <wiringPi.h>

#include "mems.h"
#include "../peripherals/spi.h"

#define FULL_RESET 0x280001
#define ENABLE_INTERNAL_REF 0x380001
#define ENABLE_DAC_CHAN 0x20000F
#define ENABLE_SOFT_LDAC 0x300000
#define VBIAS 80
#define V_DIFF_MAX 120
#define V_DIFF_TO_ANGLE_FACTOR 0.046

#define SPI_CHANNEL 2
#define GPIO39_MEMS_EN_DRV 39
//#define GPIO4_MEMS_FILT_X 4 //this should be a 60kHz clock
//#define GPIO21_MEMS_FILT_Y 21  //this should be a 60kHz clock
//#define GPIO7_MEMS_CS 7 // Temporary
//-----------------------------------------

Mems::Mems() :
	spi(SPI_CHANNEL, 250000, 1),
	enable(GPIO39_MEMS_EN_DRV),
	//clock(GPIO4_MEMS_FILT_X), // clock is not necessary on the glasses 
	//cs(GPIO7_MEMS_CS) {
	//pinMode(GPIO39_MEMS_EN_DRV, OUTPUT);
	//pinMode(GPIO12_MEMS_EN_CLK, OUTPUT);
	//pinMode(GPIO4_MEMS_FILT_X, GPIO_CLOCK); // it is starting here!
	//pinMode(GPIO21_MEMS_FILT_Y, GPIO_CLOCK);
	//pinMode(GPIO7_MEMS_CS, OUTPUT);

	//cs.write(HIGH);

	init_DAC();
	// Vref should be at 1.25V!
	send_voltage_diff_x(0);
	send_voltage_diff_y(0);
	//digitalWrite(GPIO12_MEMS_EN_CLK, HIGH);
	//gpioClockSet(GPIO4_MEMS_FILT_X, 60000);

	//clock.set_clock(60000); // clock is not necessary on the glasses 

	//gpioClockSet(GPIO21_MEMS_FILT_Y, 60000);
	//digitalWrite(GPIO39_MEMS_EN_DRV, HIGH); // TODO: Test when always high
	enable.write(HIGH); // TODO: Test when always high
}


void Mems::init_DAC() {
	send_data(FULL_RESET);
	send_data(ENABLE_INTERNAL_REF);
	send_data(ENABLE_DAC_CHAN);
	send_data(ENABLE_SOFT_LDAC);
}


void Mems::send_data(unsigned int data) {
	unsigned char send_buffer[3];

	send_buffer[0] = data >> 16 & 0xFF;
	send_buffer[1] = data >> 8 & 0xFF;
	send_buffer[2] = data & 0xFF;

	//cs.write(LOW); //temp
	//digitalWrite(GPIO7_MEMS_CS, LOW); //temp
	spi.send(send_buffer, 3);
	//digitalWrite(GPIO7_MEMS_CS, HIGH); //temp
	//cs.write(HIGH);
}


void Mems::send_voltage_diff_x(float voltage_diff) {
  voltage_diff = saturate_voltage_diff(voltage_diff);
  unsigned short bin_pos = VBIAS*65536/200 + (voltage_diff*65536/200)/2;
  unsigned short bin_neg = VBIAS*65536/200 - (voltage_diff*65536/200)/2;
  unsigned int address = (unsigned int)1 << 16;
  unsigned int command = (unsigned int)2 << 19;
  unsigned int data = command | address | bin_neg;
  send_data(bin_pos);
  send_data(data);
}

void Mems::send_voltage_diff_y(float voltage_diff) {
  voltage_diff = saturate_voltage_diff(voltage_diff);
  unsigned short bin_pos = VBIAS*65536/200 - (voltage_diff*65536/200)/2;
  unsigned short bin_neg = VBIAS*65536/200 + (voltage_diff*65536/200)/2;
  unsigned int address = (unsigned int)2 << 16;
  unsigned int data1 = address | bin_pos;
  address = (unsigned int)3 << 16;
  unsigned int command = (unsigned int)2 << 19;
  unsigned int data2 = command | address | bin_neg;
  send_data(data1);
  send_data(data2);
}

float Mems::saturate_voltage_diff(float voltage_diff) {
  if(voltage_diff > V_DIFF_MAX) {
	  voltage_diff = V_DIFF_MAX;
  } else if(voltage_diff < -V_DIFF_MAX) {
	  voltage_diff = -V_DIFF_MAX;
  }
  return voltage_diff;
}

float Mems::saturate_angle(float angle) {
	float max_angle = voltage_diff_to_angle(V_DIFF_MAX);
	if(angle > max_angle) {
		angle = max_angle;
	} else if(angle < -max_angle) {
		angle = -max_angle;
	}
	return angle;
}

void Mems::stop() {
	send_voltage_diff_x(0);
	send_voltage_diff_y(0);
	delay(1);  // necessary?
	enable.write(LOW);
	//digitalWrite(GPIO39_MEMS_EN_DRV, LOW);
}

float Mems::angle_to_voltage_diff(float angle) {
	return angle/V_DIFF_TO_ANGLE_FACTOR;
}

float Mems::voltage_diff_to_angle(float voltage_diff) {
	return voltage_diff*V_DIFF_TO_ANGLE_FACTOR;
}

void Mems::send_angle_x(float angle) {
	float voltage_diff = angle_to_voltage_diff(angle);
	send_voltage_diff_x(voltage_diff);
}

void Mems::send_angle_y(float angle) {
	float voltage_diff = angle_to_voltage_diff(angle);
	send_voltage_diff_y(voltage_diff);
}

void Mems::send_angles(float angle_x, float angle_y) {
	send_angle_x(angle_x);
	send_angle_y(angle_y);
}
