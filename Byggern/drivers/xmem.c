/*
 * xmem.c
 *
 * Created: 04/09/17 12:32:19
 *  Author: Sondre
 */ 

#include "xmem.h"

#include <avr/io.h>

#include <stdio.h>
#include <stdlib.h>

void xmem_init() {
	// Enable external memory interface
	MCUCR |= (1 << SRE);
}

void xmem_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x100;//0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n\r");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i,
			retreived_value, some_value);
			write_errors++;
		}
	}

	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed); // reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r",
			i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n\r%4d errors in write phase and \n\r%4d errors in retrieval phase\n\n\r", write_errors, retrieval_errors);
}


void xmem_set_highscore(uint32_t highscore){
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM, store it at 250
	ext_ram[highscore_adress + 0] = (highscore >> 0) & 0xFF;
	ext_ram[highscore_adress + 1] = (highscore >> 8) & 0xFF;
	ext_ram[highscore_adress + 2] = (highscore >> 16) & 0xFF;
	ext_ram[highscore_adress + 3] = (highscore >> 24) & 0xFF;
}

uint32_t xmem_get_highscore(){
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint32_t score = 0;
	score |= ext_ram[highscore_adress + 0] << 0;
	score |= ext_ram[highscore_adress + 1] << 8;
	score |= ext_ram[highscore_adress + 2] << 16;
	score |= ext_ram[highscore_adress + 3] << 24;
	return score;
}