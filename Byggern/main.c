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


#include <stdlib.h>


volatile char* sram = (char*)0x1800;
	

int main(void)
{
	usart_init(9600);

	xmem_init();
	oled_init();
	
	//joystick_init();

	//sei();

	//xmem_test();

	//joystick_calibrate_center();

    while (1) 
    {	
		int dir;
		//dir = joystick_get_direction();



		//printf("Dir: %d\n\r", dir);

		_delay_ms(10);

    }
}

USART0_RX_HANDLER() {
	char c = UDR0;
	
	printf("You pressed: %c, ( = %d)\n\r", c, c);
}
