/*
 * touch.h
 *
 * Created: 11/09/17 10:07:18
 *  Author: Sondre
 */ 


#ifndef TOUCH_H_
#define TOUCH_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	TouchSide_LEFT,
	TouchSide_RIGHT
} TouchSide_t;

void touch_init();

uint8_t touch_get_slider(TouchSide_t side);

bool touch_get_button(TouchSide_t side);


#endif /* TOUCH_H_ */