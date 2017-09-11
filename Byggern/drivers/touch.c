/*
 * touch.c
 *
 * Created: 11/09/17 10:10:02
 *  Author: Sondre
 */ 

#include "touch.h"

#include "xadc.h"

#include <avr/io.h>

#define BUTTON_DDR DDRB
#define BUTTON_PORT PINB
#define BUTTON_LEFT_PIN PB0
#define BUTTON_RIGHT_PIN PB1

#define ADC_CH_LEFT XAdcCh_CH3
#define ADC_CH_RIGHT XAdcCh_CH4

void touch_init() {
	BUTTON_DDR &= ~(1 << BUTTON_LEFT_PIN);
	BUTTON_DDR &= ~(1 << BUTTON_RIGHT_PIN);
}

uint8_t touch_get_slider(TouchSide_t side) {
	if (side == TouchSide_LEFT)
		return xadc_read(ADC_CH_LEFT);
	
	if (side == TouchSide_RIGHT)
		return xadc_read(ADC_CH_RIGHT);

	return 0;
}

bool touch_get_button(TouchSide_t side) {
	if (side == TouchSide_LEFT)
		return BUTTON_PORT & (1 << BUTTON_LEFT_PIN);
	
	if (side == TouchSide_RIGHT)
		return BUTTON_PORT & (1 << BUTTON_RIGHT_PIN);

	return false;
}
