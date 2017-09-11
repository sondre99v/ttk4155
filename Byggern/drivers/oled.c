/*
 * oled.c
 *
 * Created: 11/09/17 11:08:53
 *  Author: Sondre
 */ 

#include "oled.h"

#include <util/delay.h>

#include <stdint.h>

#include "joystick.h"

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

	_write_cmd_byte(0xA6); // Set normal display
	//_write_cmd_byte(0xA7); // Set inverted display
	
	_write_cmd_byte(0xAF); // Display on
	
	_delay_ms(10);
	

	while(1) {
		
		_write_cmd_byte(0x10);
		_write_cmd_byte(0x00);

		int a = -joystick_get_x();
		printf("%d\r\n", a);

		for(int line = 0; line < 8; line++) {
			_write_cmd_byte(0xB0 | line);


			for(int x = 0; x < a; x++) {
				_write_dat_byte(0xFF);
			}
			for(int x = a; x < 128; x++) {
				_write_dat_byte(0x00);
			}
		}

		_delay_ms(30);
	}
}
