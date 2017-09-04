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



#include <stdlib.h>




int main(void)
{
	usart_init(9600);

	xmem_init();
	
	sei();

	xmem_test();

    while (1) 
    {
		volatile char* oled_cmd = (char*)0x1000;
		volatile char* oled_data = (char*)0x1200;
		volatile char* adc = (char*)0x1400;
		volatile char* sram = (char*)0x1800;
	
		oled_cmd[4] = 5;
		oled_data[4] = 5;
		adc[4] = 5;
		sram[4] = 5;

		volatile char c;
	
		c = oled_cmd[4];
		c = oled_data[4];
		c = adc[4];
		c = sram[4];
    }
}

USART0_RX_HANDLER() {
	char c = UDR0;
	
	printf("You pressed: %c, ( = %d)\n\r", c, c);
}
