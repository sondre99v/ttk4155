/*
 * motor.c
 *
 * Created: 09/10/17 09:50:26
 *  Author: Sondre
 */ 

#include "motor.h"
#include "twi.h"
#include <avr/io.h>

#define MOTOR_DDR DDRH
#define MOTOR_PORT PORTH

#define EN_PIN 4
#define DIR_PIN 1


void motor_init() {
	TWI_Master_Initialise();
	
	MOTOR_DDR |= (1 << EN_PIN);
	MOTOR_DDR |= (1 << DIR_PIN);

	motor_disable();
}


void motor_disable() {
	MOTOR_PORT &= ~(1 << EN_PIN);
}

void motor_enable() {
	MOTOR_PORT |= (1 << EN_PIN);
}

void motor_set(uint8_t speed, motor_direction_t direction) {
	// Set direction pin
	if (direction == DIR_RIGHT) {
		MOTOR_PORT |= (1 << DIR_PIN);
	} else {
		MOTOR_PORT &= ~(1 << DIR_PIN);
	}

	// Write speed
	uint8_t buffer[] = {0x50, 0x00, speed, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00};
	TWI_Start_Transceiver_With_Data(buffer, 9);
}
