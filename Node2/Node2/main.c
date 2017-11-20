/*
 * Node2.c
 *
 * Created: 25/09/17 12:43:33
 * Author : Sondre
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>

#include "drivers/spi.h"
#include "drivers/mcp2515.h"
#include "drivers/can.h"
#include "drivers/pwm.h"
#include "drivers/adc.h"
#include "drivers/motor.h"
#include "drivers/shooter.h"
#include "drivers/song.h"

int main(void)
{
	spi_init();
	can_init();
	pwm_init();
	adc_init();
	adc_init_channel(AdcCh_CH0);
	motor_init();
	shooter_init();
	sei();

	pwm_set_servo_deflection(50);
	

	// Home the motor to the left edge
	motor_enable();
	motor_set(100, DIR_LEFT);

	_delay_ms(200);
	motor_reset_encoder();
	
	int consecutive_same = 0;
	int16_t stationary_value = 0;
	while(consecutive_same < 10) {
		int16_t encoder_reading = motor_read_encoder();
		if (encoder_reading == stationary_value) {
			consecutive_same++;
		} else {
			stationary_value = encoder_reading;
		}
		_delay_ms(20);
	}
	
	motor_set(0, DIR_LEFT);

	motor_reset_encoder();
	
	// Define constants and variable for the regulator
	float position_reference = 0;
	float integrated_error = 0;
	float prev_position_error = 0;
	float velocity = 0.0;

	const float Kp = 0.1 * 33.94;
	const float Ki = 0.0002 * 33.94;
	const float Kd = 1 * 33.94;
	const uint8_t deadband = 20;

	while (1)
	{
		// Run regulator
		float enc_pos = motor_read_encoder();
		float position = 255.0 * enc_pos / -8657.0;
		float position_error = position_reference - position;
		
		integrated_error += position_error;
		velocity = 0.95 * velocity + 0.05 * (position_error - prev_position_error);

		float voltage = Ki * integrated_error + Kp * position_error + Kd * velocity;

		prev_position_error = position_error;

		bool voltage_negative = (voltage < 0);

		if (voltage_negative) {
			voltage = -voltage;
		}

		voltage += deadband;

		if (voltage > 200) {
			voltage = 200;
		}

		motor_set((uint8_t)voltage, voltage_negative ? DIR_LEFT : DIR_RIGHT);

		
		// Receive a can message
		CanFrame_t frame;
		if (can_rx_message(&frame)) {
			if (frame.data.u8[3] == 1) {
				song_play(SONG_END);
			} else if (frame.data.u8[3] == 2) {
				song_play(SONG_START);
			} else if (frame.data.u8[3] == 3) {
				song_play(SONG_LOADING);
			} else {
				// Exctract values from can message
				int8_t joystick_x = frame.data.i8[0];
				uint8_t slider = frame.data.u8[1];
				uint8_t should_shoot = frame.data.u8[2];


				// Set servo deflection
				int16_t servo_defl = 100 * (joystick_x - 40 + 127) / 255;
			
				if (servo_defl > 100) servo_defl = 100;
				if (servo_defl < 0) servo_defl = 0;

				pwm_set_servo_deflection(servo_defl);
			

				// Shoot if needed
				if (should_shoot) {
					shooter_shoot();
				}
			

				// Set the position reference for the motor
				position_reference = slider;
				if (position_reference < 5) position_reference = 5;
				if (position_reference > 250) position_reference = 250;

			
				// Send the return message
				frame.id = 0x120;
				frame.length = 0x2;
			
				frame.data.u8[0] = adc_read(AdcCh_CH0);
				frame.data.u8[1] = position;

				can_tx_message(&frame);
			}
		}
		
		_delay_ms(10);
    }
}
