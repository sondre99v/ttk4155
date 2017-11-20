/*
 * usart.c
 *
 * Created: 04/09/17 10:58:21
 *  Author: Sondre
 */ 

#include "usart.h"

#include <stdio.h>
#include <avr/io.h>

static void _usart_tx_char(char c)
{
	while (!(UCSR0A & (1 << UDRE0))) { }; // Wait until data register is empty
	UDR0 = c;
}


void usart_init(uint32_t baud_rate)
{
	uint16_t ubrr = (F_CPU / 16) / baud_rate - 1;

	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr & 0x00FF;

	// Enable transmit only
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

	// Set frame format
	UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

	// Enable receive interrupt
	UCSR0B |= (1 << RXCIE0);
}

void usart_print_str(char* str) {
	for (char* p = str; *p; p++) {
		_usart_tx_char(*p);
	}
}

void _usart_print_nibble(uint8_t value) {
	if (value < 10) {
		_usart_tx_char('0' + value);
	} else {
		_usart_tx_char('A' + value - 10);
	}
}

void usart_print_uint8(uint8_t value) {
	uint8_t most_significant_nibble = value >> 4;
	uint8_t least_significant_nibble = value & 0x0F;
	
	usart_print_str("0x");
	_usart_print_nibble(most_significant_nibble);
	_usart_print_nibble(least_significant_nibble);
}

void usart_print_uint16(uint16_t value) {
	usart_print_str("0x");
	_usart_print_nibble((value & 0xF000) >> 12);
	_usart_print_nibble((value & 0x0F00) >> 8);
	_usart_print_nibble((value & 0x00F0) >> 4);
	_usart_print_nibble((value& 0x000F) >> 0);
}
