/*
 * shooter.c
 *
 * Created: 09/10/17 10:07:27
 *  Author: Sondre
 */ 

#include "shooter.h"

#include <avr/io.h>
#include <util/delay.h>


#define SHOOTER_DDR DDRF
#define SHOOTER_PORT PORTF
#define SHOOTER_PIN 1


void shooter_init() {
	SHOOTER_DDR |= (1 << SHOOTER_PIN);
	SHOOTER_PORT &= ~(1 << SHOOTER_PIN);
}

void shooter_shoot() {
	SHOOTER_PORT |= (1 << SHOOTER_PIN);
	_delay_ms(SHOOTER_PULSE_DURATION_MS);
	SHOOTER_PORT &= ~(1 << SHOOTER_PIN);
}