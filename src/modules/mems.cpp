/*
 *  @file mems.cpp
 *  @author Patrice Buteau
 *  @version 28 juin 2019
 *
 * NOTE : Before you can use SPI interface, you may need to use
 * the gpio utility to load the SPI drivers into the
 * kernel: gpio load spi
 */
#include <iostream>

#include <wiringPi.h>

#include "mems.h"
#include "../peripherals/spi.h"

#define FULL_RESET 0x280001
#define ENABLE_INTERNAL_REF 0x380001
#define ENABLE_DAC_CHAN 0x20000F
#define ENABLE_SOFT_LDAC 0x300000
#define VBIAS 80
//#define V_DIFF_MAX 120
#define V_DIFF_MAX 75
#define V_DIFF_TO_ANGLE_FACTOR 0.046

#define SPI_CHANNEL 0
#define GPIO45_MEMS_EN_DRV 45
//#define GPIO4_MEMS_FILT_X 4 //this should be a 60kHz clock
//-----------------------------------------

using namespace std;

Mems::Mems() :
	spi(SPI_CHANNEL, 250000, 1),
	enable(GPIO45_MEMS_EN_DRV) {
	//clock(GPIO4_MEMS_FILT_X),
	angle_x = 0;
	angle_y = 0;
	init_DAC();
	// Vref should be at 1.25V!
	send_voltage_diff_x(0);
	send_voltage_diff_y(0);
	//clock.set_clock(60000);
	enable.write(HIGH); // TODO: Test when always high
}


void Mems::init_DAC() {
	send_data(FULL_RESET);
	send_data(ENABLE_INTERNAL_REF);
	send_data(ENABLE_DAC_CHAN);
	send_data(ENABLE_SOFT_LDAC);
}


void Mems::send_data(unsigned int data) {
	unsigned char send_buffer[3] = {0,0,0};

	send_buffer[0] = data >> 16 & 0xFF;
	send_buffer[1] = data >> 8 & 0xFF;
	send_buffer[2] = data & 0xFF;

	spi.send(send_buffer, 3);
}

void Mems::send_voltage_diff_x(float voltage_diff) {
  voltage_diff = saturate_voltage_diff(voltage_diff);
  
  unsigned short bin_pos = VBIAS*65536/200 + (voltage_diff*65536/200)/2;
  unsigned short bin_neg = VBIAS*65536/200 - (voltage_diff*65536/200)/2;
  unsigned int address = (unsigned int)1 << 16;
  unsigned int command = (unsigned int)2 << 19;
  unsigned int data = command | address | bin_neg;
  
  send_data(bin_pos);
  delayMicroseconds(2);
  send_data(data);
}

void Mems::send_voltage_diff_y(float voltage_diff) {
  voltage_diff = saturate_voltage_diff(voltage_diff);
  //cout << voltage_diff << endl;
  unsigned short bin_pos = VBIAS*65536/200 - (voltage_diff*65536/200)/2;
  unsigned short bin_neg = VBIAS*65536/200 + (voltage_diff*65536/200)/2;
  unsigned int address = (unsigned int)2 << 16;
  unsigned int data1 = address | bin_pos;
  address = (unsigned int)3 << 16;
  unsigned int command = (unsigned int)2 << 19;
  unsigned int data2 = command | address | bin_neg;
  
  send_data(data1);
  delayMicroseconds(2);
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
}

float Mems::angle_to_voltage_diff(float angle) {
	return angle/V_DIFF_TO_ANGLE_FACTOR;
}

float Mems::voltage_diff_to_angle(float voltage_diff) {
	return voltage_diff*V_DIFF_TO_ANGLE_FACTOR;
}

float Mems::send_angle_x(float angle) {
	angle_x = saturate_angle(angle);
	float voltage_diff = angle_to_voltage_diff(angle);
	send_voltage_diff_x(voltage_diff);
	return angle_x;
}

float Mems::send_angle_y(float angle) {
	angle_y = saturate_angle(angle);
	//cout << angle_y << endl;
	float voltage_diff = angle_to_voltage_diff(angle);
	send_voltage_diff_y(voltage_diff);
	return angle_y;
}

void Mems::send_angles(float angle_x, float angle_y) {
	send_angle_x(angle_x);
	send_angle_y(angle_y);
}

void Mems::print_angles() {
  cout << "X : " << angle_x << ", Y : " << angle_y << endl;
}

float Mems::get_angle_x() {
  return angle_x;
}

float Mems::get_angle_y() {
  return angle_y;
}
