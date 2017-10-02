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
		_delay_ms(481);

		CanFrame_t frame = {
			.id = 0x123,
			.length = 0x1,
			.data.u32[0] = count
		};

		can_tx_message(&frame);
		frame.data.u8[0] = 0;

		_delay_ms(10);
		
		if(can_rx_message(&frame)) {
			display_clear();
			display_set_position(0, 0);
			//printf("TX data = 0x%02X", count);
			printf("RX data = 0x%02X", frame.data.u8[0]);
		}
		
		display_set_position(0, 1);
		printf("Err: 0x%02X", mcp_read(0x0E));

	    display_repaint();

		count++;
    }
}

ISR(__vector_default) {
	_delay_ms(1000);
}
