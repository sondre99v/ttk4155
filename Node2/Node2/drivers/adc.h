/*
 * adc.h
 *
 * Created: 02/10/17 13:17:45
 *  Author: Sondre
 */ 


#ifndef ADC_H_
#define ADC_H_


#include <stdint.h>


typedef enum {
	AdcCh_CH0 = 0x00,
	AdcCh_CH1 = 0x01,
	AdcCh_CH2 = 0x02,
	AdcCh_CH3 = 0x03,
	AdcCh_CH4 = 0x04,
	AdcCh_CH5 = 0x05,
	AdcCh_CH6 = 0x06,
	AdcCh_CH7 = 0x07,
	AdcCh_CH8 = 0x20,
	AdcCh_CH9 = 0x21,
	AdcCh_CH10 = 0x22,
	AdcCh_CH11 = 0x23,
	AdcCh_CH12 = 0x24,
	AdcCh_CH13 = 0x25,
	AdcCh_CH14 = 0x26,
	AdcCh_CH15 = 0x27
} AdcCh_t;


void adc_init();
void adc_init_channel(AdcCh_t channel);
uint16_t adc_read(AdcCh_t channel);


#endif /* ADC_H_ */