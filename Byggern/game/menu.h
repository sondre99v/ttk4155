/*
 * menu.h
 *
 * Created: 18/09/17 09:56:53
 *  Author: Sondre
 */ 


#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

void menu_init();
int8_t menu_handle_input();
uint8_t menu_get_sound_request();

#endif /* MENU_H_ */