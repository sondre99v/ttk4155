/*
 * oled.c
 *
 * Created: 11/09/17 11:08:53
 *  Author: Sondre
 */ 

#include "oled.h"

#include "font.h"

#include <util/delay.h>
#include <avr/pgmspace.h>

#include <stdint.h>
#include <stdbool.h>

#include "joystick.h"

static uint8_t cursor_row = 0;
static uint8_t cursor_column = 0;

volatile uint8_t* OLED_CMD_ADDR = (uint8_t*)0x1000;
volatile uint8_t* OLED_DAT_ADDR = (uint8_t*)0x1200;

void _write_cmd_byte(uint8_t value) {
	*OLED_CMD_ADDR = value;
	_delay_us(1);
}

void _write_dat_byte(uint8_t value) {
	*OLED_DAT_ADDR = value;
	_delay_us(1);
}

void oled_init() {
	_write_cmd_byte(0xAE); // Display off
	_write_cmd_byte(0xA1); // Segment remap
	_write_cmd_byte(0xDA); // Common pads hardware: alternative
	_write_cmd_byte(0x12);
	_write_cmd_byte(0xC8); // Common output scan direction:com63~com0
	_write_cmd_byte(0xA8); // Multiplex ration mode:63
	_write_cmd_byte(0x3F);
	_write_cmd_byte(0xD5); // Display divide ratio/osc. freq. mode
	_write_cmd_byte(0x80);
	_write_cmd_byte(0x81); // Contrast control
	_write_cmd_byte(0x50);
	_write_cmd_byte(0xD9); // Set pre-charge period
	_write_cmd_byte(0x21);
	_write_cmd_byte(0x20); // Set Memory Addressing Mode
	_write_cmd_byte(0x02);
	_write_cmd_byte(0xDB); // VCOM deselect level mode
	_write_cmd_byte(0x30);
	_write_cmd_byte(0xAD); // Master configuration
	_write_cmd_byte(0x00);
	_write_cmd_byte(0xA4); // Out follows RAM content
	_write_cmd_byte(0xA6); // Set non-inverted display
	_write_cmd_byte(0xAF); // Display on
	oled_clear();
}

void oled_clear() {
	_write_cmd_byte(0x10);
	_write_cmd_byte(0x00);

	for(int line = 0; line < 8; line++) {
		_write_cmd_byte(0xB0 | line);

		for(int x = 0; x < 128; x++) {
			_write_dat_byte(0x00);
		}
	}

	oled_home();
}

void oled_home() {
	_write_cmd_byte(0xB0);

	cursor_column = 0;
	cursor_row = 0;
}

bool oled_position(uint8_t row, uint8_t column) {
	if (row > 7 || row < 0 || column > 127 || column < 0) return false;
	
	_write_cmd_byte(0xB0 | row);
	
	uint8_t low_column = column % 16;
	uint8_t high_column = ((column- low_column) / 16);
	
	_write_cmd_byte(0x10 | high_column);
	_write_cmd_byte(low_column);

	cursor_column = column;
	cursor_row = row;
	
	return true;
}

void oled_clear_row(uint8_t row) {
	oled_position(row, 0);
	for (uint8_t i = 0; i < 180; i++) {
		_write_dat_byte(0x00);
	}
}

void oled_write_char(char c) {
	if (c == '\n') {
		cursor_column = 0;
		cursor_row++;
		oled_position(cursor_row, cursor_column);

		if (cursor_row >= SCREEN_ROWS) {
			oled_clear();
			cursor_row = 0;
		}
	} else {
		if (cursor_column >= SCREEN_COLUMNS) {
			cursor_column = 0;
			cursor_row++;

			if (cursor_row >= SCREEN_ROWS) {
				//oled_clear();
				cursor_row = 0;
			}

			oled_position(cursor_row, cursor_column);
		}
		
		int index = c & 0x7F;
		bool inverted = (c & 0x80) != 0;

		for(int col = 0; col < 8; col++) {
			if (inverted) {
				_write_dat_byte(~pgm_read_byte(&(font8x8_basic[(int)index][col])));
			} else {
				_write_dat_byte(pgm_read_byte(&(font8x8_basic[(int)index][col])));
			}
		}

		cursor_column++;
	}
}
