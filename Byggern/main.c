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

#include "drivers/spi.h"

#include "drivers/mcp2515.h"

#include "game/menu.h"


#include <stdlib.h>

//volatile char* sram = (char*)0x1800;
	

int main(void)
{
	spi_init();

	usart_init(9600);

	xmem_init();

	display_init();

	display_clear();
	
	//menu_init();

	//joystick_init();
	//joystick_calibrate_center();

	//sei();

	mcp_reset();
	

	mcp_write(0x2A, 0x30); // Write Config 1
	mcp_write(0x29, 0x00); // Write Config 2
	mcp_write(0x28, 0x02); // Write Config 3
	
	mcp_write(0x2B, 0x00); // Disable all interrupts
	mcp_write(0x2C, 0x00); // Clear all interrupt flags
	mcp_write(0x2D, 0x00); // Clear all error flags
	
	mcp_write(0x0C, 0x00); // Clear BFPCTRL
	mcp_write(0x0D, 0x00); // Clear TXRTSCTRL


	// Setup TX buffer 0
	mcp_write(0x30, 0x03); // Set message to highest priority, and clear the request to send flag

	mcp_write(0x31, 0x00); // Set standard id to 0x014
	mcp_write(0x32, 0x14);
	
	mcp_write(0x33, 0x00); // Set extended id bits to 0
	mcp_write(0x34, 0x00);
	
	mcp_write(0x35, 0x08); // Set data length to 8, and mode to normal frame
	
	mcp_write(0x36, 0xCA); // Set data to send
	mcp_write(0x37, 0xFE);
	mcp_write(0x38, 0xBA);
	mcp_write(0x39, 0xBE);
	mcp_write(0x3A, 0x01);
	mcp_write(0x3B, 0x23);
	mcp_write(0x3C, 0x45);
	mcp_write(0x3D, 0x67);
	
	mcp_write(0x0F, 0x44); // Enable can controller with loopback

	_delay_ms(10);

	// Send
	mcp_request_to_send(true, false, false);

    while (1)
    {
		uint8_t data = mcp_read(0x30);

		display_set_position(0, 0);
		printf("Reg[0x30] = 0x%02X", data);

	    display_repaint();

		_delay_ms(200);
    }
}

USART0_RX_HANDLER() {
	char c = UDR0;
	
	printf("You pressed: %c, ( = %d)\n\r", c, c);
}
