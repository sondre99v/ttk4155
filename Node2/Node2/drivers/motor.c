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
#include <stdbool.h>

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



//Define regulator parameters
const float Kp = 0.1 * 33.94;
const float Ki = 0.0002 * 33.94;
const float Kd = 1 * 33.94;

//Define regulator constants
const uint8_t deadband = 20;
float integrated_error = 0;
float prev_position_error = 0;
float velocity = 0.0;


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


void motor_calibrate_position(){
		// Home the motor to the left edge
		motor_enable();
		motor_set(100, DIR_LEFT);

		_delay_ms(200);
		motor_reset_encoder();
		
		int consecutive_same = 0;
		int16_t stationary_value = 0;
		while(consecutive_same < 10) {
			int16_t encoder_reading = motor_get_position();
			if (encoder_reading == stationary_value) {
				consecutive_same++;
				} else {
				stationary_value = encoder_reading;
			}
			_delay_ms(20);
		}
		
		motor_set(0, DIR_LEFT);
}


void motor_regulate(float encoder_position, float slider){
	
	if (slider < 5) slider = 5;
	if (slider > 250) slider = 250;
	
	float error = slider - encoder_position;
	integrated_error += error;
	velocity = 0.95 * velocity + 0.05 * (error - prev_position_error);

	float voltage = Ki * integrated_error + Kp * error + Kd * velocity;

	prev_position_error = error;

	bool voltage_negative = (voltage < 0);

	if (voltage_negative) {
		voltage = -voltage;
	}

	voltage += deadband;

	if (voltage > 200) {
		voltage = 200;
	}

	motor_set((uint8_t)voltage, voltage_negative ? DIR_LEFT : DIR_RIGHT);
}


void motor_reset_encoder() {
	// Pulse !reset pin
	ENCODER_PORT &= ~(1 << ENCODER_RST_PIN);
	_delay_us(10);
	ENCODER_PORT |= (1 << ENCODER_RST_PIN);
}

float motor_get_position() {
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
	
	//Calculate scaled encoder position
	float enc_pos = (msb << 8) | lsb;
	return 255.0 * enc_pos / -8657.0;
}