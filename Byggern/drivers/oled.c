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
	
	oled_clear();
}

void oled_clear(void){
	_write_cmd_byte(0x10);
	_write_cmd_byte(0x00);

	for(int line = 0; line < 8; line++) {
		_write_cmd_byte(0xB0 | line);

		for(int x = 0; x < 128; x++) {
			_write_dat_byte(0x00);
		}
	}
}

void oled_home(void){
	_write_cmd_byte(0xB0);
}

bool oled_position(uint8_t row, uint8_t column){
	if (row > 7 || row < 0 || column > 127 || column < 0) return false;
	_write_cmd_byte(0xB0 | row);
	uint8_t low_column = column % 16;
	uint8_t high_column = ((column- low_column) / 16);
	_write_cmd_byte(0x10 | high_column);
	_write_cmd_byte(low_column);
	//_write_dat_byte(0xFF);
	return true;
	}

void oled_clear_row(uint8_t row){
	oled_position(row,0);
	for (uint8_t i = 0; i < 180; i++){
		_write_dat_byte(0x00);
	}
}

void oled_print(char a){
	
	_write_dat_byte(0x00);
	switch (a){
		case 1:
			_write_dat_byte(0b11000000);
			_write_dat_byte(0b00110000);
			_write_dat_byte(0b00011100);
			_write_dat_byte(0b00010011);
			_write_dat_byte(0b00011100);
			_write_dat_byte(0b00110000);
			_write_dat_byte(0b11000000);
		break;
		case 2:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b10010001);
			_write_dat_byte(0b10011001);
			_write_dat_byte(0b01100110);
		break;
		case 3:
			_write_dat_byte(0b00111100);
			_write_dat_byte(0b01000010);
			_write_dat_byte(0b10000001);
			_write_dat_byte(0b01000010);
			break;
		case 4:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b10000001);
			_write_dat_byte(0b10000001);
			_write_dat_byte(0b01000010);
			_write_dat_byte(0b00111100);
		break;
		case 5:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b10001001);
			_write_dat_byte(0b10001001);
			_write_dat_byte(0b10001001);
		break;
		case 6:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b00010001);
			_write_dat_byte(0b00010001);
			_write_dat_byte(0b00010001);
		break;
		case 7:
			_write_dat_byte(0b01111110);
			_write_dat_byte(0b10000001);
			_write_dat_byte(0b10000001);
			_write_dat_byte(0b10010001);
			_write_dat_byte(0b01110001);
		break;
		case 8:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b00010000);
			_write_dat_byte(0b00010000);
			_write_dat_byte(0b11111111);
		break;
		case 9:
			_write_dat_byte(0x81);
			_write_dat_byte(0xFF);
			_write_dat_byte(0x81);
		break;
		case 10:
			_write_dat_byte(0b01000001);
			_write_dat_byte(0b10000001);
			_write_dat_byte(0b01111111);		
		break;
		case 11:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b00011000);
			_write_dat_byte(0b01100110);
			_write_dat_byte(0b10000001);
		break;
		case 12:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b10000000);
			_write_dat_byte(0b10000000);
			_write_dat_byte(0b10000000);
		break;
		case 13:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b00001100);
			_write_dat_byte(0b00110000);
			_write_dat_byte(0b00001100);
			_write_dat_byte(0b11111111);
		break;
		case 14:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b00000110);
			_write_dat_byte(0b00011000);
			_write_dat_byte(0b01100000);
			_write_dat_byte(0b11111111);
		break;
		case 15:
			_write_dat_byte(0b00111100);
			_write_dat_byte(0b01000010);
			_write_dat_byte(0b10000001);
			_write_dat_byte(0b01000010);
			_write_dat_byte(0b00111100);
		break;
		case 16:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b00010001);
			_write_dat_byte(0b00010001);
			_write_dat_byte(0b00001110);
		break;
		case 17:
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b00110001);
			_write_dat_byte(0b01010001);
			_write_dat_byte(0b10001110);
		break;
		case 18:
			_write_dat_byte(0b10000100);
			_write_dat_byte(0b10001010);
			_write_dat_byte(0b01010001);
			_write_dat_byte(0b00100001);
		break;
		case 19:
			_write_dat_byte(0b00000001);
			_write_dat_byte(0b00000001);
			_write_dat_byte(0b11111111);
			_write_dat_byte(0b00000001);
			_write_dat_byte(0b00000001);
		break;
		case 20:
			_write_dat_byte(0b01111111);
			_write_dat_byte(0b10000000);
			_write_dat_byte(0b10000000);
			_write_dat_byte(0b10000000);
			_write_dat_byte(0b01111111);
		break;
		case 21:
			_write_dat_byte(0b00011111);
			_write_dat_byte(0b01100000);
			_write_dat_byte(0b10000000);
			_write_dat_byte(0b01100000);
			_write_dat_byte(0b00011111);
		break;
		case 22:
			_write_dat_byte(0b00011111);
			_write_dat_byte(0b01100000);
			_write_dat_byte(0b10000000);
			_write_dat_byte(0b01100000);
			_write_dat_byte(0b00011100);
			_write_dat_byte(0b01100000);
			_write_dat_byte(0b10000000);
			_write_dat_byte(0b01100000);
			_write_dat_byte(0b00011111);
		break;
		}
	_write_dat_byte(0x00);
}

/*
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
	*/