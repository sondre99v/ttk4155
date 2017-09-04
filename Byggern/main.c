/*
 * ByggernTestProject1.c
 *
 * Created: 04/09/17 09:39:57
 * Author : Sondre
 */ 

#define F_CPU 4915200


#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdio.h>


void usart_tx_char(char c)
{
	while (!(UCSR0A & (1 << UDRE0))) { }; // Wait until data register is empty
	UDR0 = c;
}

void usart_tx_str(char* str)
{
	for(char* ptr = str; *ptr; ptr++)
	usart_tx_char(*ptr);
}


void usart_init()
{
	const uint32_t baud_rate = 9600;
	uint16_t ubrr = (F_CPU / 16) / baud_rate - 1;

	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr & 0x00FF;

	// Enable transmit only
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);

	// Set frame format
	UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

	// Enable receive interrupt
	UCSR0B |= (1 << RXCIE0);

	fdevopen(usart_tx_char, NULL);
}


int main(void)
{
	usart_init();
	sei();

    while (1) 
    {
		_delay_ms(500);
    }
}

ISR(USART0_RXC_vect) {
	char c = UDR0;
	
	printf("You pressed: %c, ( = %d)\n\r", c, c);
}
