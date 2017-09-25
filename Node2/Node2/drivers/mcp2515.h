/*
 * mcp2515.h
 *
 * Created: 18/09/17 11:45:22
 *  Author: Sondre
 */ 


#ifndef MCP2515_H_
#define MCP2515_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint8_t CAN0_ReceiveInterrupt : 1;
	uint8_t CAN1_ReceiveInterrupt : 1;
	uint8_t CAN0_TransmissionPending : 1;
	uint8_t CAN0_TransmitInterrupt : 1;
	uint8_t CAN1_TransmissionPending : 1;
	uint8_t CAN1_TransmitInterrupt : 1;
	uint8_t CAN2_TransmissionPending : 1;
	uint8_t CAN2_TransmitInterrupt : 1;
} McpStatus_t;

void mcp_reset();

uint8_t mcp_read(uint8_t addr);
void mcp_write(uint8_t addr, uint8_t value);
void mcp_modify_bit(uint8_t addr, uint8_t mask, uint8_t value);

void mcp_request_to_send(bool buffer0, bool buffer1, bool buffer2);
McpStatus_t mcp_read_status();


#endif /* MCP2515_H_ */