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
	
    while (1) 
    {
		CanFrame_t frame;
		while(!can_rx_message(&frame)) { };

		frame.id = 0x002;
		frame.data.u8[0] ^= 0xFF;

		can_tx_message(&frame);
    }
}
