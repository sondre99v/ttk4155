/*
 * usart.h
 *
 * Created: 04/09/17 10:58:31
 *  Author: Sondre
 */ 


#ifndef USART_H_
#define USART_H_

#include <stdint.h>

#define USART0_RX_HANDLER() ISR(USART0_RXC_vect)

void usart_init(uint32_t baud_rate);
void usart_print_str(char* str);
void usart_print_uint8(uint8_t value);
void usart_print_uint16(uint16_t value);

#endif /* USART_H_ */