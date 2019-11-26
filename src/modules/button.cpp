/*
 *  @file button.cpp
 *  @author Patrice Buteau
 *  @version 9 octobre 2019
 */
#include <iostream>
#ifdef __arm__
#include <wiringPi.h>
#endif
#include "button.h"

using namespace std;

Button::Button(int button) :
 	input(button) {
#ifdef __arm__
    input.set_pull(PUD_UP);
	button_number = button;
	button_state = NOT_PRESSED;
#endif
}


int Button::scan_button() {
#ifdef __arm__
	int scan_result = input.read();
	//cout << scan_result << endl;
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
#endif
    return 0;
}





