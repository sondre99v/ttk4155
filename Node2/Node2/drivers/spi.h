/*
 * spi.h
 *
 * Created: 18/09/17 10:43:26
 *  Author: Sondre
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

void spi_init();
void spi_trancieve(uint8_t* tx_buffer, uint8_t* rx_buffer, int length);


#endif /* SPI_H_ */