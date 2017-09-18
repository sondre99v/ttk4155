/*
 * extmem.h
 *
 * Created: 04/09/17 12:32:09
 *  Author: Sondre
 */ 


#ifndef EXTMEM_H_
#define EXTMEM_H_

// Address space allocations
#define display_text_start ((char*)0x1800)
#define display_text_length 128



void xmem_init();

void xmem_test();


#endif /* EXTMEM_H_ */