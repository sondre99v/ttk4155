/*
 * xadc.c
 *
 * Created: 04/09/17 15:04:09
 *  Author: Sondre
 */ 

#include "xadc.h"

#include <util/delay.h>

#define ADC_ADDR ((uint8_t*) 0x1400)
#define ADC_CONVERSION_TIME_uS 50

uint8_t xadc_read(XAdcCh_t channel) {
	*ADC_ADDR = channel;
	
	_delay_us(ADC_CONVERSION_TIME_uS);

	return *ADC_ADDR;
}
