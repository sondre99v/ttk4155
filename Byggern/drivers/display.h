/*
 * display.h
 *
 * Created: 18/09/17 09:13:35
 *  Author: Sondre
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

typedef enum {
	COLOR_NORMAL,
	COLOR_HIGHLIGHT
} TextColor_t;

void display_init();

void display_clear();

void display_set_position(uint8_t column, uint8_t row);

void display_set_color(TextColor_t color);

void display_repaint();


#endif /* DISPLAY_H_ */