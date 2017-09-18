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

#include "game/menu.h"


#include <stdlib.h>


//volatile char* sram = (char*)0x1800;
	

int main(void)
{
	PORTE |= 1;
	
	usart_init(9600);

	xmem_init();

	display_init();

	display_clear();
	
	menu_init();

	//oled_init();
	
	joystick_init();
	joystick_calibrate_center();

	//sei();

	//xmem_test();

    while (1) 
    {
		int choice = menu_handle_input();

		if (choice != -1) {
			display_set_position(0, 6);
			display_set_color(COLOR_NORMAL);
			printf("Selected: %d", choice);
		}

	    display_repaint();

		_delay_ms(20);
    }
}

USART0_RX_HANDLER() {
	char c = UDR0;
	
	printf("You pressed: %c, ( = %d)\n\r", c, c);
}
