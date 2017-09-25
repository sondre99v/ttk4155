/*
 * usart.c
 *
 * Created: 04/09/17 10:58:21
 *  Author: Sondre
 */ 

#include "usart.h"

#include <stdio.h>
#include <avr/io.h>

static int _usart_tx_char(char c, FILE* stream)
{
	while (!(UCSR0A & (1 << UDRE0))) { }; // Wait until data register is empty
	UDR0 = c;
	return 1;
}


void usart_init(uint32_t baud_rate)
{
	uint16_t ubrr = (F_CPU / 16) / baud_rate - 1;

	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr & 0x00FF;

	// Enable transmit and receive
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

	// Set frame format
	UCSR0C = /*(1 << URSEL0) |*/ (1 << USBS0) | (3 << UCSZ00);

	// Enable receive interrupt
	UCSR0B |= (1 << RXCIE0);
	
	fdevopen(_usart_tx_char, NULL);
}
