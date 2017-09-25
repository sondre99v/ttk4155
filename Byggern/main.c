/*
 * Byggern.c
 *
 * Created: 04/09/17 09:39:57
 * Author : Sondre
 */ 

#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "drivers/xmem.h"
#include "drivers/xadc.h"
#include "drivers/touch.h"
#include "drivers/display.h"
#include "drivers/joystick.h"
#include "drivers/can.h"

#include "drivers/spi.h"

#include "drivers/mcp2515.h"

#include "game/menu.h"


#include <stdlib.h>

//volatile char* sram = (char*)0x1800;

int main(void)
{
	spi_init();
	xmem_init();
	display_init();
	can_init();

	sei();

	for(int i=0;i<101;i++) {
		
		display_set_position(0, 0);
		printf("Loading: %d%%", i);
		display_repaint();
		//_delay_ms(5);
	}

	uint8_t count = 0;
    while (1)
    {
		_delay_ms(100);

		CanFrame_t frame = {
			.id = 0x7FF,
			.length = 0x01,
			.data.u8[0] = 0xF0
		};

		can_tx_message(&frame);
		frame.data.u8[0] = 0;

		_delay_ms(10);
		display_clear();

		if(can_rx_message(&frame)) {
			display_set_position(0, 0);
			printf("TX data = 0x%02X", count);
			display_set_position(0, 1);
			printf("RX data = 0x%02X", frame.data.u8[0]);
		} else {
			display_set_position(0, 0);
			printf("No RX");
		}

	    display_repaint();

		count++;
    }
}

ISR(__vector_default) {
	_delay_ms(1000);
}
