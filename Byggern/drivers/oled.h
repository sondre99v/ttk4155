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

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TEXT_WIDTH 8
#define TEXT_HEIGHT 8
#define SCREEN_COLUMNS (SCREEN_WIDTH / TEXT_WIDTH)
#define SCREEN_ROWS (SCREEN_HEIGHT / TEXT_HEIGHT)

typedef enum {
	COMP_NONE,
	COMP_SCALE,
	COMP_RUN_LENGTH
} Compression_t;

typedef struct {
	uint8_t width;
	uint8_t height;
	Compression_t compression;
	const uint8_t* img_data;
} Image_t;

void oled_init();
void oled_clear();
void oled_home();
bool oled_position(uint8_t row, uint8_t column);
void oled_clear_row(uint8_t row);

void oled_write_char(char c);
void oled_draw_image(Image_t* image, uint8_t x, uint8_t row);

#endif /* OLED_H_ */