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

int main(void)
{
	CanFrame_t frame;

	spi_init();
	can_init();
	pwm_init();
	adc_init();
	adc_init_channel(AdcCh_CH0);
	motor_init();
	shooter_init();

	sei();

	
	motor_enable();
	
	
	while (1)
	{
		int size = sizeof(melody) / sizeof(int);
		for (int thisNote = 0; thisNote < size; thisNote++) {
			
			// to calculate the note duration, take one second
			// divided by the note type.
			//e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
			int noteDuration = 1000 / tempo[thisNote];
			
			buzz(melody[thisNote], noteDuration);
			
			// to distinguish the notes, set a minimum time between them.
			// the note's duration + 30% seems to work well:
			int pauseBetweenNotes = noteDuration * 1.30;
			delay_var_us(1000 * pauseBetweenNotes);
			
			// stop the tone playing:
			//buzz(0, noteDuration);
			
		}
	}
	

	bool shot = false;

	float position_reference = 0;
	motor_reset_encoder();
	float integrated_error = 0;
	
	const float Kp = 0.1;
	const float Ki = 0.0002;
	const float Kd = 0.001;

	uint8_t deadband = 55;

	float prev_position_error = 0;
	float velocity = 0.0;


	while (1)
	{

		// Run regulator
		float position = motor_read_encoder();
		float position_error = position_reference - position;
		integrated_error += position_error;
		
		velocity = 0.95 * velocity + 0.05 * (position_error - prev_position_error);

		float voltage = Ki * integrated_error + Kp * position_error + Kd * velocity;

		prev_position_error = position_error;

		bool direction = (voltage < 0);

		if (direction) {
			voltage = -voltage;
		}

		if (voltage > 250 - deadband) {
			voltage = 250 - deadband;
		}

		motor_set((uint8_t)voltage + deadband, direction ? DIR_RIGHT : DIR_LEFT);


		if (can_rx_message(&frame)) {
			int16_t joystick_pos = (int16_t)(frame.data.i8[0]);
			//int16_t shoot_frequency = 100 * (int16_t)(frame.data.i8[1] + 100) / 255;

			int16_t servo_defl = 100 * joystick_pos / 255;
			
			position_reference = joystick_pos * 40;

			if (servo_defl > 100) servo_defl = 100;
			if (servo_defl < 0) servo_defl = 0;

			pwm_set_servo_deflection(servo_defl);
			
			if (frame.data.u8[2] > 0) {
				if (!shot) {
					shooter_shoot();
					shot = true;
				}
			} else {
				shot = false;
			}


			frame.id = 0x120;
			frame.length = 0x4;
			
			frame.data.i16[0] = position;//adc_read(AdcCh_CH0);
			frame.data.i16[1] = (int16_t)position_reference;

			can_tx_message(&frame);
		}
		
		_delay_ms(10);
    }
}
