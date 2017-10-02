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

int main(void)
{
	spi_init();
	can_init();
	pwm_init();
	adc_init();
	//adc_init_channel(AdcCh_CH8);

	sei();

	CanFrame_t frame;

	while (1)
	{
		_delay_ms(10);

		if (can_rx_message(&frame)) {
			int16_t joystick_pos = (int16_t)(frame.data.i8[0] + 100);

			int16_t servo_defl = 100 * joystick_pos / 255;
			
			if (servo_defl > 100) servo_defl = 100;
			if (servo_defl < 0) servo_defl = 0;

			pwm_set_servo_deflection(servo_defl);
			
			frame.id = 0x120;
			frame.length = 0x4;
			
			frame.data.u16[0] = adc_read(AdcCh_CH8);
			frame.data.i16[1] = servo_defl;

			can_tx_message(&frame);
		}
    }
}
