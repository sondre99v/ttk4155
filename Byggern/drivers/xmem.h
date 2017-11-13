/*
 * extmem.h
 *
 * Created: 04/09/17 12:32:09
 *  Author: Sondre
 */ 


#ifndef EXTMEM_H_
#define EXTMEM_H_

#include <stdint.h>

// Address space allocations
#define display_text_start ((char*)0x1800)
#define display_text_length 128
#define highscore_adress 250

void xmem_init();
void xmem_set_highscore(uint32_t highscore);
uint32_t xmem_get_highscore();

void xmem_test();


#endif /* EXTMEM_H_ */