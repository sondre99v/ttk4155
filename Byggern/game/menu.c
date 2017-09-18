/*
 * menu.c
 *
 * Created: 18/09/17 09:59:04
 *  Author: Sondre
 */ 

#include "menu.h"
#include "../drivers/joystick.h"
#include "../drivers/display.h"

#include <stdio.h>
#include <stdbool.h>

const char* menu_heading = "Select item";
const char* menu_choices[4] = {
	"Item0",
	"Item1",
	"Item2",
	"Item3"
};
const int menu_choices_num = 4;

static int selected_choice = 0;

static bool waiting_for_joystick_return = false;


void _menu_paint_choices() {
	for(int i = 0; i < menu_choices_num; i++) {
		display_set_position(3, i + 1);
		display_set_color(i == selected_choice ? COLOR_HIGHLIGHT : COLOR_NORMAL);
		printf("%s", menu_choices[i]);
	}
}

void menu_init() {
	display_clear();
	display_set_position(1, 0);
	printf("%s", menu_heading);
	_menu_paint_choices();
}

int8_t menu_handle_input() {
	JoystickDir_t dir = joystick_get_direction();
	
	if (dir == JoystickDir_CENTER) {
		waiting_for_joystick_return = false;
	}

	if (waiting_for_joystick_return) return -1;


	if (dir == JoystickDir_UP && selected_choice > 0) {
		selected_choice--;
		waiting_for_joystick_return = true;
	}

	if (dir == JoystickDir_DOWN && selected_choice < menu_choices_num - 1) {
		selected_choice++;
		waiting_for_joystick_return = true;
	}

	_menu_paint_choices();

	if (joystick_get_button()) {
		return selected_choice;
	}

	return -1;
}