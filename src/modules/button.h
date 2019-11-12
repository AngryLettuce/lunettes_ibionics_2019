/*
 *  @file button.h
 *	@brief Everything related to buttons.
 *  @author Patrice Buteau
 *  @version 9 octobre 2019
 */

#ifndef SRC_MODULES_BUTTON_H_
#define SRC_MODULES_BUTTON_H_

#include "../peripherals/digital_in.h"

#define NOT_PRESSED     0
#define PRESSED         1
#define HELD_DOWN       2
// NEXT GPIO DEFINES ARE FROM THE DEVKIT!
//#define GPIO5_BUTTON1   5
//#define GPIO6_BUTTON2   6
//#define GPIO13_BUTTON3 13
//#define GPIO19_BUTTON4 19
/* HERE ARE THE GOOD GPIOs from our own PCB*/
#define GPIO7_BUTTON1 7
#define GPIO6_BUTTON2 6
#define GPIO5_BUTTON3 5
#define GPIO4_BUTTON4 4

class Button {
public:
	/**
	 * @brief Button constructor.
	 * @param button corresponding button number from the defines.
	 */
	Button(int button);
	/**
	 * @brief Scan the button state.
	 * @return 0 if not pressed, 1 if pressed, 2 if held down
	 */
	int scan_button();
private:
	Digital_in input;
	int button_state;
	int button_number;
};


#endif /* SRC_MODULES_BUTTON_H_ */
