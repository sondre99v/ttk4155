/*
 * mcp2515.c
 *
 * Created: 18/09/17 12:05:38
 *  Author: Sondre
 */ 

#include "mcp2515.h"
#include "spi.h"
#include <stdlib.h>
#include <util/delay.h>


typedef enum {
	MCP_CMD_WRITE = 0x02,
	MCP_CMD_READ = 0x03,
	MCP_CMD_BIT_MODIFY = 0x05,
	MCP_CMD_REQ_SEND_base = 0x10,
	MCP_CMD_READ_STATUS = 0xA0,
	MCP_CMD_RESET = 0xC0
} McpCommand_t;

void mcp_reset() {
	uint8_t buffer[] = {MCP_CMD_RESET};
	spi_trancieve(buffer, NULL, 1);
}

uint8_t mcp_read(uint8_t addr) {
	uint8_t buffer[] = {MCP_CMD_READ, addr, 0x00};
	spi_trancieve(buffer, buffer, 3);
	return buffer[2];
}


void mcp_write(uint8_t addr, uint8_t value) {
	uint8_t buffer[] = {MCP_CMD_WRITE, addr, value};
	spi_trancieve(buffer, NULL, 3);
}

void mcp_modify_bit(uint8_t addr, uint8_t mask, uint8_t value) {
	uint8_t buffer[] = {MCP_CMD_BIT_MODIFY, addr, mask, value};
	spi_trancieve(buffer, NULL, 4);
}

void mcp_request_to_send(bool buffer0, bool buffer1, bool buffer2) {
	uint8_t cmd = MCP_CMD_REQ_SEND_base;
	if (buffer0) cmd |= (1 << 0);
	if (buffer1) cmd |= (1 << 1);
	if (buffer2) cmd |= (1 << 2);

	uint8_t buffer[] = {cmd};

	spi_trancieve(buffer, NULL, 1);
}

McpStatus_t mcp_read_status() {
	// Retry until read succeeds
	while (true) {
		uint8_t buffer[] = {MCP_CMD_READ_STATUS, 0x00, 0x00};
		spi_trancieve(buffer, buffer, 3);

		if (buffer[1] == buffer[2]) {
			return *((McpStatus_t*)(buffer + 1));
		}
	}
}
