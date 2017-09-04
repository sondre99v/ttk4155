/*
 * xadc.c
 *
 * Created: 04/09/17 15:04:09
 *  Author: Sondre
 */ 

#include "xadc.h"
#include <stdint.h>

typedef enum {
	XAdcCh_CH1,
	XAdcCh_CH2,
	XAdcCh_CH3,
	XAdcCh_CH4,
} XAdcCh_t;

void xadc_init();

uint8_t xadc_read();