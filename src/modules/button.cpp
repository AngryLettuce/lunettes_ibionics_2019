/*
 *  @file button.cpp
 *  @author Patrice Buteau
 *  @version 9 octobre 2019
 */
#include <wiringPi.h>

#include "button.h"


Button::Button(int button) :
 	input(button) {
    input.set_pull(PUD_UP);
	button_number = button;
	button_state = NOT_PRESSED;
}


int Button::scan_button() {
	int scan_result = input.read();
	if(scan_result == HIGH) {
		button_state = NOT_PRESSED;
	}
	else if(scan_result == LOW && button_state == NOT_PRESSED) {
		button_state = PRESSED;
	}
	else {
		button_state = HELD_DOWN;
	}
	return button_state;
}





