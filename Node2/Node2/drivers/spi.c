/*
 * spi.c
 *
 * Created: 18/09/17 10:43:38
 *  Author: Sondre
 */ 

#include "spi.h"
#include <stdlib.h>
#include <avr/io.h>

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SS_PIN 0
#define SS_PIN 7
#define MOSI_PIN 2
#define MISO_PIN 3
#define SCK_PIN 1

void spi_init() {
	SPI_DDR |= (1 << SPI_SS_PIN) | (1 << SS_PIN) | (1 << MOSI_PIN) | (1 << SCK_PIN);
	SPI_DDR &= ~(1 << MISO_PIN);
	SPI_PORT |= (1 << SPI_SS_PIN);
	SPI_PORT |= (1 << SS_PIN);

	PRR0 &= ~(1 << PRSPI);

	SPCR = (1 << MSTR) | (1<< CPHA) | (1 << CPOL) | (1 << SPE) | (1 << SPR0);
}

void spi_trancieve(uint8_t* tx_buffer, uint8_t* rx_buffer, int length) {
	SPI_PORT &= ~(1 << SS_PIN);
	
	for(int i = 0; i < length; i++) {
		SPDR = tx_buffer[i];
		
		// Wait for word transmission to complete
		while (!(SPSR & (1 << SPIF))) { };

		if (rx_buffer != NULL) {
			rx_buffer[i] = SPDR;
		}
	}

	SPI_PORT |= (1 << SS_PIN);
}
