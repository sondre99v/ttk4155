/*
 * joystick.h
 *
 * Created: 11/09/17 09:48:15
 *  Author: Sondre
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	JoystickDir_CENTER,
	JoystickDir_RIGHT,
	JoystickDir_UP,
	JoystickDir_LEFT,
	JoystickDir_DOWN 
} JoystickDir_t;


void joystick_init();
void joystick_calibrate_center();
int8_t joystick_get_x();
int8_t joystick_get_y();
bool joystick_get_button();
JoystickDir_t joystick_get_direction();

#endif /* JOYSTICK_H_ */