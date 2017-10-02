/*
 * pwm.c
 *
 * Created: 02/10/17 11:42:44
 *  Author: Sondre
 */ 

#include "pwm.h"
#include <avr/io.h>

#define PWM_DDR DDRB
#define PWM_PORT PORTB
#define PWM_PIN PB6

#define TC_TOP 40000

void pwm_init() {
	// Set TOP value for counter
	ICR1 = TC_TOP;
	
	// Enable output pin, and set compare value
	PWM_DDR |= (1 << PWM_PIN);
	pwm_set_duty_cycle(50);

	// Enable counter in correct mode
	TCCR1A = (1 << WGM11) | (0 << WGM10) | (1 << COM1B1) | (0 << COM1B0);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
}

void pwm_set_duty_cycle(uint8_t duty_cycle) {
	OCR1B = (uint16_t)((uint32_t)duty_cycle * TC_TOP / 100);
}


uint16_t prev_pulse_width;

void pwm_set_pulse_width(uint16_t pulse_width_us) {
	if (pulse_width_us < 900) {
		OCR1B = 2 * 900;
	} else if (pulse_width_us > 2100) {
		OCR1B = 2 * 2100;
	} else {
		OCR1B = (pulse_width_us * 2);
	}
}

void pwm_set_servo_deflection(uint8_t deflection) {
	const int min_pulse_us = 900;
	const int max_pulse_us = 2100;
	const int max_deflection = 100;
	
	pwm_set_pulse_width(min_pulse_us + 
		(max_pulse_us - min_pulse_us) * (uint32_t)deflection / max_deflection);
}
