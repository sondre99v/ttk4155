/*
 * joystick.c
 *
 * Created: 11/09/17 09:51:13
 *  Author: Sondre
 */ 

#include "joystick.h"

#include "xadc.h"

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BUTTON_PORT PORTB
#define BUTTON_PINR PINB
#define BUTTON_PIN 0

#define ADC_CH_X XAdcCh_CH2
#define ADC_CH_Y XAdcCh_CH1

#define clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define DIRECTION_THRESHOLD 100

static int8_t calibration_offset_x = 0;
static int8_t calibration_offset_y = 0;


void joystick_init() {
	DDRE &= (1 << BUTTON_PIN);
}

void joystick_calibrate_center() {
	calibration_offset_x = 0;
	calibration_offset_y = 0;
	calibration_offset_x = joystick_get_x();
	calibration_offset_y = joystick_get_y();
}

int8_t joystick_get_x() {
	int16_t adc_val = ((int)xadc_read(ADC_CH_X) - 128);
	adc_val -= calibration_offset_x;

	return clamp(adc_val, -127, 128);
}

int8_t joystick_get_y() {
	int16_t adc_val = ((int)xadc_read(ADC_CH_Y) - 128);
	adc_val -= calibration_offset_y;

	return clamp(adc_val, -127, 128);

}

bool joystick_get_button() {
	return (BUTTON_PINR & (1 << BUTTON_PIN)) == 0;
}

JoystickDir_t joystick_get_direction() {
	int8_t x = joystick_get_x();
	int8_t y = joystick_get_y();

	if (x > DIRECTION_THRESHOLD && abs(x) > abs(y)) return JoystickDir_RIGHT;
	if (y > DIRECTION_THRESHOLD && abs(x) < abs(y)) return JoystickDir_UP;
	if (x < -DIRECTION_THRESHOLD && abs(x) > abs(y)) return JoystickDir_LEFT;
	if (y < -DIRECTION_THRESHOLD && abs(x) < abs(y)) return JoystickDir_DOWN;

	return JoystickDir_CENTER;
}
