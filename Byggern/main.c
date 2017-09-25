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

#include "drivers/usart.h"
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
	_delay_ms(50);
	spi_init();
	usart_init(9600);
	xmem_init();
	display_init();
	can_init();

	sei();

	for(int i=0;i<101;i++) {
		
		display_set_position(0, 0);
		printf("Loading: %d%%", i);
		display_repaint();
		_delay_ms(5);
	}

	uint8_t count = 0;
    while (1)
    {
		_delay_ms(100);

		CanFrame_t frame = {
			.id = 0x014,
			.length = 0x01,
			.data.u8[0] = count
		};

		can_tx_message(&frame);
		frame.data.u8[0] = 0;

		_delay_ms(10);

		if(can_rx_message(&frame)) {
			display_set_position(0, 0);
			printf("RX data = 0x%02X", frame.data.u8[0]);
		} else {
			display_set_position(0, 0);
			printf("No RX");
		}

	    display_repaint();

		count++;
    }
}

USART0_RX_HANDLER() {
	char c = UDR0;
	
	printf("You pressed: %c, ( = %d)\n\r", c, c);
}

ISR(__vector_default) {
	_delay_ms(1000);
}
