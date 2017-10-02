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

	joystick_calibrate_center();

	sei();

	for(int i=0;i<101;i++) {
		
		display_set_position(0, 0);
		printf("Loading: %d%%", i);
		display_repaint();
		//_delay_ms(5);
	}

	int score = 10;
	int holdoff = 0;

    while (1)
    {
		//_delay_ms(10);

		CanFrame_t frame = {
			.id = 0x100,
			.length = 0x2,
			.data.u8[0] = joystick_get_x(),
			.data.u8[1] = joystick_get_y()
		};

		can_tx_message(&frame);
		frame.data.u8[0] = 0;
		
		if(can_rx_message(&frame)) {
			display_clear();
			display_set_position(0, 0);
			printf("VALUE = 0x%04X", frame.data.u16[0]);
			display_set_position(0, 1);
			printf("POS = %d", frame.data.i16[1]);
			
			display_set_position(0, 3);
			printf("SCORE: %d", score);

			if (holdoff > 0) {
				holdoff--;
			} else {
				if (frame.data.u16[0] < 0x20) {
					score--;
					if (score < 0) {
						score = 10;
					}
					holdoff = 100;
				}
			}

			display_repaint();
		}
    }
}

ISR(__vector_default) {
	_delay_ms(1000);
}
