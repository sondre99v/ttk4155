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

#define SERVO_DEFLECTION_OFFSET 50

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

	pwm_set_servo_deflection(SERVO_DEFLECTION_OFFSET);
	
	motor_calibrate_position();
	
	motor_reset_encoder();
	
	//Declaration of game variables
	int8_t joystick_x = 0;
	uint8_t slider_data = 0;
	uint8_t should_shoot = 0;


	while (1)
	{
		motor_regulate(motor_get_position(), slider_data);
		
		CanFrame_t frame;
		if (can_rx_message(&frame)) {
			switch (frame.data.u8[3]){
				case 1:
					song_play(SONG_END);
					break;
				case 2:
					song_play(SONG_START);
					break;
				case 3:
					song_play(SONG_LOADING);
					break;
				case 4:
					song_play(SONG_BEEP);
					break;
				default:
					joystick_x = frame.data.i8[0];
					slider_data = frame.data.u8[1];
					should_shoot = frame.data.u8[2];
					
					pwm_set_servo_deflection(joystick_x);
					
					if (should_shoot) shooter_shoot();
					
					// Send the return message
					frame.id = 0x120;
					frame.length = 0x2;
					
					frame.data.u8[0] = adc_read(AdcCh_CH0);
					frame.data.u8[1] = motor_get_position();
					can_tx_message(&frame);
					
					break;
			}
		}
		
		_delay_ms(10);
    }
}
