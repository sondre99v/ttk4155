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
	
volatile uint8_t rx_data = 0;

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
		_delay_ms(20);
	}

	uint8_t count = 0;
    while (1)
    {
	
		_delay_ms(1000);

		CanFrame_t frame = {
			.id = 0x014,
			.length = 0x01,
			.data.u8[0] = count
		};

		can_tx_message(frame);
		


		display_set_position(0, 0);
		printf("RX data = 0x%02X", rx_data);
		
	    display_repaint();

		count++;
    }
}

void can_rx_callback(CanFrame_t rx_frame) {
	rx_data = rx_frame.data.u8[0];
}

USART0_RX_HANDLER() {
	char c = UDR0;
	
	printf("You pressed: %c, ( = %d)\n\r", c, c);
}

ISR(__vector_default) {
	_delay_us(50);
}
