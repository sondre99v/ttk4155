/*
 * xadc.h
 *
 * Created: 04/09/17 15:03:59
 *  Author: Sondre
 */ 


#ifndef XADC_H_
#define XADC_H_

#include <stdint.h>

typedef enum {
	XAdcCh_CH1 = 0x4,
	XAdcCh_CH2 = 0x5,
	XAdcCh_CH3 = 0x6,
	XAdcCh_CH4 = 0x7
} XAdcCh_t;

uint8_t xadc_read(XAdcCh_t channel);


#endif /* XADC_H_ */