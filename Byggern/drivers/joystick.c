/*
 * joystick.c
 *
 * Created: 11/09/17 09:51:13
 *  Author: Sondre
 */ 

#include "joystick.h"

#include "xadc.h"

#include <math.h>

#define ADC_CH_X XAdcCh_CH1
#define ADC_CH_Y XAdcCh_CH2

#define clamp(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

static int8_t calibration_offset_x = 0;
static int8_t calibration_offset_y = 0;

void joystick_calibrate_center() {
	calibration_offset_x = 0;
	calibration_offset_y = 0;
	calibration_offset_x = joystick_get_x();
	calibration_offset_y = joystick_get_y();
}

int8_t joystick_get_x() {
	int adc_val = xadc_read(ADC_CH_X);
	adc_val -= calibration_offset_x;
	
	return clamp(adc_val, -127, 128);
}

int8_t joystick_get_y() {
	int adc_val = xadc_read(ADC_CH_Y);
	adc_val -= calibration_offset_y;

	return clamp(adc_val, -127, 128);

}

bool joystick_get_button() {
	// TODO: Implement
	return false;
}

JoystickDir_t joystick_get_direction() {
	int8_t x = joystick_get_x();
	int8_t y = joystick_get_y();
	
	if (x > 0 && abs(x) > abs(y)) return DIR_RIGHT;
	if (y < 0 && abs(x) < abs(y)) return DIR_UP;
	if (x < 0 && abs(x) > abs(y)) return DIR_LEFT;
	if (y > 0 && abs(x) < abs(y)) return DIR_DOWN;

	return DIR_CENTER;
}