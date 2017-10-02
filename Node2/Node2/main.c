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

int main(void)
{
	spi_init();
	can_init();

	sei();

	CanFrame_t frame;

	while (1)
	{
		_delay_ms(10);

		if (can_rx_message(&frame)) {
			frame.id = 0x120;
			
			for(int i = 0; i < frame.length; i++) {
				frame.data.u8[i] = ~frame.data.u8[i];
			}

			can_tx_message(&frame);
		}
    }
}
