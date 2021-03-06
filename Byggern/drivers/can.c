/*
 * can.c
 *
 * Created: 25/09/17 10:05:59
 *  Author: Sondre
 */ 

#include "can.h"
#include "mcp2515.h"
#include <avr/io.h>


void can_init() {
	mcp_reset();

	mcp_write(0x0F, 0x80); // Enter configuration mode

	mcp_write(0x2A, (0 << 6) | 0x20); // Write Config 1
	mcp_write(0x29, 0x92); // Write Config 2
	mcp_write(0x28, 0xC2); // Write Config 3
	
	mcp_write(0x2B, 0x00); // Disable all interrupts
	mcp_write(0x2C, 0x00); // Clear all interrupt flags
	mcp_write(0x2D, 0x00); // Clear all error flags
	
	mcp_write(0x0C, 0x00); // Disable all interrupts
	mcp_write(0x0D, 0x00); // Clear TXRTSCTRL
	
	// Setup RX buffer 0
	mcp_write(0x60, 0x00); // Write config
	
	// Setup RX buffer 1
	mcp_write(0x70, 0x00); // Write config
	
	mcp_write(0x04, 0xFF); // Write config RX buffer 0
	mcp_write(0x05, 0xE0); // Write config RX buffer 0
	mcp_write(0x24, 0xFF); // Write config RX buffer 0
	mcp_write(0x25, 0xE0); // Write config RX buffer 0


	mcp_write(0x20, 0x00); // Clear ID masks
	mcp_write(0x21, 0x00); // Clear ID masks
	mcp_write(0x22, 0x00); // Clear ID masks
	mcp_write(0x23, 0x00); // Clear ID masks
	

	mcp_write(0x0F, 0x04); // Enable can controller
}

void can_tx_message(CanFrame_t* tx_frame) {
	
	// Setup TX buffer 0
	mcp_write(0x30, 0x03); // Set message to highest priority, and clear the request to send flag

	mcp_write(0x31, (tx_frame->id >> 3) & 0xFF); // Set standard id
	mcp_write(0x32, (tx_frame->id << 5) & 0xE0);
	
	mcp_write(0x35, tx_frame->length); // Set data length, and mode to normal frame
	
	for(int i = 0; i < tx_frame->length; i++) {
		mcp_write(0x36 + i, tx_frame->data.u8[i]);
	}

	mcp_request_to_send(true, false, false);
}

bool can_rx_message(volatile CanFrame_t* rx_frame) {
	McpStatus_t stat = mcp_read_status();
	if (!stat.CAN0_ReceiveInterrupt) {
		return false;
	}
	
	rx_frame->id = (mcp_read(0x61) << 3) & 0x07F8;
	rx_frame->id |= (mcp_read(0x62) >> 5);

	rx_frame->length = mcp_read(0x65);
	
	if (rx_frame->length > 8) {
		rx_frame->length = 8;
	}

	for(int i = 0; i < rx_frame->length; i++) {
		rx_frame->data.u8[i] = mcp_read(0x66 + i);
	}

	mcp_modify_bit(0x2C, 0x01, 0x00);

	return true;
}
