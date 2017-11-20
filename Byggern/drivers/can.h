/*
 * can.h
 *
 * Created: 25/09/17 10:05:50
 *  Author: Sondre
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <stdint.h>
#include <stdbool.h>

typedef union {
	uint8_t u8[8];
	int8_t i8[8];
	uint16_t u16[4];
	int16_t i16[4];
	uint32_t u32[2];
	int32_t i32[2];
	uint64_t u64;
	int64_t i64;
} CanData_t;

typedef struct {
	uint16_t id;
	uint8_t length;
	CanData_t data;
} CanFrame_t;


void can_init();

void can_tx_message(CanFrame_t* tx_frame);

bool can_rx_message(volatile CanFrame_t* rx_frame);



#endif /* CAN_H_ */