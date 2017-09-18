/*
 * display.c
 *
 * Created: 18/09/17 09:18:01
 *  Author: Sondre
 */ 

#include "display.h"
#include "oled.h"
#include "xmem.h"

static uint8_t cursor_column;
static uint8_t cursor_row;
static bool text_inverted = false;

int _put_char(char c, FILE* stream) {
	if (c == '\n') {
		cursor_column = 0;
		cursor_row++;

		if (cursor_row >= SCREEN_ROWS) {
			cursor_row = 0;
		}
	} else {
		if (cursor_column >= SCREEN_COLUMNS) {
			cursor_column = 0;
			cursor_row++;

			if (cursor_row >= SCREEN_ROWS) {
				cursor_row = 0;
			}
		}

		if (text_inverted) {
			c |= (1 << 7);
		}
		
		display_text_start[cursor_row * SCREEN_COLUMNS + cursor_column] = c;
	}

	cursor_column++;

	return 1;
}

void display_init() {
	oled_init();
	display_clear();
	
	fdevopen(_put_char, NULL);
}

void display_clear() {
	for(int i = 0; i < display_text_length; i++) {
		display_text_start[i] = '\0';
	}

	cursor_column = 0;
	cursor_row = 0;
}

void display_set_position(uint8_t column, uint8_t row) {	
	cursor_column = column;
	cursor_row = row;
}

void display_set_color(TextColor_t color) {
	if (color == COLOR_NORMAL) {
		text_inverted = false;
	} else if (color == COLOR_HIGHLIGHT) {
		text_inverted = true;
	}
}

void display_repaint() {
	for(int row = 0; row < SCREEN_ROWS; row++) {
		oled_position(row, 0);
		for (int col = 0; col < SCREEN_COLUMNS; col++) {
			oled_write_char(display_text_start[row * SCREEN_COLUMNS + col]);
		}
	}
}