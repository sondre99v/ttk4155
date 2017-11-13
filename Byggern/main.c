/*
 * Byggern.c
 *
 * Created: 04/09/17 09:39:57
 * Author : Sondre
 */ 

#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "drivers/xmem.h"
#include "drivers/xadc.h"
#include "drivers/touch.h"
#include "drivers/display.h"
#include "drivers/joystick.h"
#include "drivers/can.h"

#include "drivers/spi.h"

#include "drivers/mcp2515.h"

#include "game/menu.h"

#include <stdlib.h>


#define LOOP_UPDATE_FREQUENCY_HZ 30
#define TIMER_TARGET_VALUE (F_CPU / (LOOP_UPDATE_FREQUENCY_HZ * 64))

//volatile char* sram = (char*)0x1800;

int main(void)
{
	spi_init();
	xmem_init();
	display_init();
	display_clear();
	can_init();
	joystick_init();
	joystick_calibrate_center();
	sei();

	// Init timer counter with prescaler /64
	TCCR1B = 0x03; 

	for(int i=0;i<101;i++) {
		
		display_set_position(0, 0);
		printf("Loading: %d%%", i);
		display_repaint();
	}

	int score = 10;
	int holdoff = 0;

	bool joystick_button_prev = false;

	int counter = 0;


	// Define menu variables
	typedef enum {
		MENU_NONE = 0,
		MENU_MAIN, 
		MENU_PLAY_GAME, 
		MENU_HIGHSCORE, 
		MENU_STATUS
	} Menu;
	
	Menu menu_current = MENU_MAIN;
	Menu menu_select = MENU_PLAY_GAME;
	
    while (1)
    {
	    // Reset loop timer to zero
	    TCNT1 = 0;

	    counter++;

		// Run menu handling
		switch (menu_current) {
			case MENU_MAIN:
				display_clear();
				display_set_position(0, 0);
				printf("Main menu:");
				display_set_position(0, 1);
				display_set_color(COLOR_HIGHLIGHT);
				printf("Play game");
				display_set_color(COLOR_NORMAL);
				display_set_position(0, 2);
				printf("Highest score");
				display_set_position(0, 3);
				printf("Status");
				
				switch (joystick_get_direction()) {
					case JoystickDir_DOWN:
						menu_select++;
						if (menu_select == MENU_NONE) {
							menu_select = MENU_PLAY_GAME;
						}
						if (menu_select == MENU_PLAY_GAME) {
							display_set_position(0, 1);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Play game");
							display_set_color(COLOR_NORMAL);
						}
						if (menu_select == MENU_HIGHSCORE) {
							display_set_position(0, 2);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Highest score");
							display_set_color(COLOR_NORMAL);
						}
						if (menu_select == MENU_STATUS) {
							display_set_position(0, 3);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Status");
							display_set_color(COLOR_NORMAL);
						}
						break;
					case JoystickDir_UP:
						menu_select --;
						if (menu_select == MENU_NONE) {
							menu_select = MENU_STATUS;
						}
						if (menu_select == MENU_PLAY_GAME) {
							display_set_position(0, 1);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Play game");
							display_set_color(COLOR_NORMAL);
						}
						if (menu_select == MENU_HIGHSCORE) {
							display_set_position(0, 2);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Highest score");
							display_set_color(COLOR_NORMAL);
						}
						if (menu_select == MENU_STATUS) {
							display_set_position(0, 3);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Status");
							display_set_color(COLOR_NORMAL);
						}
						break;
					default: break;
				}
				if (joystick_get_direction() == JoystickDir_RIGHT){
					menu_current = menu_select;
				}
				break;
			case MENU_PLAY_GAME:
				display_clear();
				display_set_position(0, 0);
				printf("Game simulation:");
				display_set_position(0, 1);
				break;
			case MENU_HIGHSCORE:
				display_clear();
				display_set_position(0, 0);
				printf("Highest score: %lu", xmem_get_highscore()); //Reads highest score from memory
				
				if (joystick_get_direction() == JoystickDir_LEFT){
					menu_current = MENU_MAIN;
					break;
				}
				display_repaint();
				break;
			case MENU_STATUS:				
				display_clear();
				display_set_position(0, 0);
				printf("Status:");
				display_set_position(0, 1);
				if (1){					//CAN OK
				printf("CAN: OK");
				} else {
					printf("CAN: ERROR");
				}
				display_set_position(0, 2);
				if (1){					//Node 2 OK
					printf("Node 2: OK");
					} else {
					printf("Node 2: ERROR");
				}
				display_set_position(0, 3);
				if (1){					//IO OK
					printf("I/O board: OK");
					} else {
					printf("I/O board: ERROR");
				}
				display_set_position(0, 4);
				if (1){					//Encryption OK
					printf("Encryption: OK");
					} else {
					printf("Encryption: ERROR");
				}
				if (joystick_get_direction() == JoystickDir_LEFT){
					menu_current = MENU_MAIN;
					break;
				}
				display_repaint();
				break;
			default:
				menu_current = MENU_MAIN; 
		}
		
		
		// Read sensors
		int8_t joystick_x = joystick_get_x();
		uint8_t slider = touch_get_slider(TouchSide_RIGHT);
		bool joystick_button = !joystick_get_button();


		// Send CAN message to Node 2
		uint8_t button_mesage = 0;
		if (joystick_button && !joystick_button_prev) {
			button_mesage = 1;
		}
		joystick_button_prev = joystick_button;
		
		uint8_t position_request = slider + 30 * sin((float)counter / 30 * 6.28);
		uint8_t angle_request = joystick_x + 30 * sin((float)counter / 30 * 6.28 / 1.3);

		CanFrame_t frame = {
			.id = 0x100,
			.length = 0x3,
			.data.u8[0] = angle_request,
			.data.u8[1] = position_request,
			.data.u8[2] = button_mesage
		};

		can_tx_message(&frame);

		
		// Receive CAN message
		if(can_rx_message(&frame)) {
			// Update display
			display_clear();
			display_set_position(0, 0);
			printf("JOYSTICK = %d", joystick_x);
			display_set_position(0, 1);
			printf("SLIDER = %d", slider);
			
			display_set_position(0, 3);
			
			
			printf("SCORE: %u", counter / LOOP_UPDATE_FREQUENCY_HZ);

			display_repaint();
			
			if (holdoff > 0) {
				holdoff--;
				} else {
				if (frame.data.u16[0] < 0x20) {
					score--;
					if (score < 0) {
						score = 10;
					}
					holdoff = 100;
				}
			}

		}

		// Wait for timer to reach value corresponding to 
		// desired update frequency
		while (TCNT1 < TIMER_TARGET_VALUE) { } ;
    }
}

ISR(__vector_default) {
	_delay_ms(1000);
}
