/*
 * adc.c
 *
 * Created: 02/10/17 13:17:53
 *  Author: Sondre
 */ 

#include "adc.h"
#include <avr/io.h>

void _set_channel(AdcCh_t ch) {
	ADMUX &= ~0x1F;
	ADMUX |= ((int)ch) & 0x1F;

	ADCSRB &= ~(1 << MUX5);
	ADCSRB |= ((int)ch >> 5) << MUX5;
}

void adc_init() {
	ADMUX = (1 << REFS0);
	ADCSRB = (1 << MUX5);
	//_set_channel(0);

	ADCSRA = (1 << ADEN);
}

void adc_init_channel(AdcCh_t channel) {
	volatile int channel_number = (int)channel & 0b000111;
	channel_number |= (channel >> 2) & 0b001000;
	
	if (channel_number < 8) {
		DIDR0 |= (1 << channel_number);
		DDRF &= ~(1 << channel_number);
	} else {
		DIDR2 |= (1 << (channel_number - 8));
		DDRK &= ~(1 << (channel_number - 8));
	}
}

uint16_t adc_read(AdcCh_t channel) {
	_set_channel(channel);

	ADCSRA |= (1 << ADSC);

	// Wait for finished conversion
	while (!(ADCSRA & (1 << ADIF))) { };
	
	return ADC;
}
