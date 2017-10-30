/*
 * motor.c
 *
 * Created: 09/10/17 09:50:26
 *  Author: Sondre
 */ 

#include "motor.h"
#include "twi.h"
#include <avr/io.h>
#include <util/delay.h>

#define MOTOR_DDR DDRH
#define MOTOR_PORT PORTH

#define EN_PIN 4
#define DIR_PIN 1

#define ENCODER_DDR DDRH
#define ENCODER_PORT PORTH
#define ENCODER_DATA PINK

#define ENCODER_NOE_PIN 5
#define ENCODER_SEL_PIN 3
#define ENCODER_RST_PIN 6


void motor_init() {
	ENCODER_DDR |= (1 << ENCODER_NOE_PIN);
	ENCODER_DDR |= (1 << ENCODER_SEL_PIN);
	ENCODER_DDR |= (1 << ENCODER_RST_PIN);
	ENCODER_PORT |= (1 << ENCODER_NOE_PIN);
	ENCODER_PORT &= ~(1 << ENCODER_SEL_PIN);
	ENCODER_PORT |= (1 << ENCODER_RST_PIN);

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

void motor_reset_encoder() {
	// Pulse !reset pin
	ENCODER_PORT &= ~(1 << ENCODER_RST_PIN);
	_delay_us(10);
	ENCODER_PORT |= (1 << ENCODER_RST_PIN);
}

int16_t motor_read_encoder() {
	// Enable output
	ENCODER_PORT &= ~(1 << ENCODER_NOE_PIN);
	
	// Select MSB
	ENCODER_PORT &= ~(1 << ENCODER_SEL_PIN);

	_delay_us(20);

	int8_t msb = ENCODER_DATA;
	
	// Select LSB
	ENCODER_PORT |= (1 << ENCODER_SEL_PIN);
	
	_delay_us(20);

	uint8_t lsb = ENCODER_DATA;

	// Disable output
	ENCODER_PORT |= (1 << ENCODER_NOE_PIN);
	
	return (msb << 8) | lsb;
}
