/*
 * oled.h
 *
 * Created: 11/09/17 11:09:04
 *  Author: Sondre
 */ 

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef OLED_H_
#define OLED_H_

void oled_init(void);
void oled_clear(void);
bool oled_position(uint8_t row, uint8_t column);
void oled_clear_row(uint8_t row);
void oled_print(char a);

/*void oled_print(char* str);*/

#endif /* OLED_H_ */