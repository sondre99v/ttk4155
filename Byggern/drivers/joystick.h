/*
 * joystick.h
 *
 * Created: 11/09/17 09:48:15
 *  Author: Sondre
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>

typedef enum {
	DIR_CENTER,
	DIR_RIGHT,
	DIR_UP,
	DIR_LEFT,
	DIR_DOWN 
} JoystickDir_t;

void joystick_calibrate_center();
int8_t joystick_get_x();
int8_t joystick_get_y();
bool joystick_get_button();
JoystickDir_t joystick_get_direction();

#endif /* JOYSTICK_H_ */