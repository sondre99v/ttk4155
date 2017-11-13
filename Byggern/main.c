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

	for(int i=0;i<101;i++) {
		
		display_set_position(0, 0);
		printf("Loading: %d%%", i);
		display_repaint();
		//_delay_ms(5);
	}

	int score = 10;
	int holdoff = 0;
	
	enum Menu {main_menu, play_game, highscore, status, no_menu};
	
	Menu menu_current = main_menu;
	Menu menu_select = play_game;
	
    while (1)
    {
		//_delay_ms(10);
		switch (menu_current) {
			case main_menu:
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
				
				switch (joystick_get_direction()){
					case JoystickDir_DOWN:
						menu_select ++;
						if (menu_select == no_menu) {
							menu_select = play_game;
						}
						if menu_select == play_game{
							display_set_position(0, 1);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Play game");
							display_set_color(COLOR_NORMAL);
						}
						if menu_select == highscore{
							display_set_position(0, 2);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Highest score");
							display_set_color(COLOR_NORMAL);
						}
						if menu_select == status{
							display_set_position(0, 3);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Status");
							display_set_color(COLOR_NORMAL);
						}
						break;
					case JoystickDir_UP:
						menu_select --;
						if (menu_select == no_menu) {
							menu_select = status;
						}
						if menu_select == play_game{
							display_set_position(0, 1);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Play game");
							display_set_color(COLOR_NORMAL);
						}
						if menu_select == highscore{
							display_set_position(0, 2);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Highest score");
							display_set_color(COLOR_NORMAL);
						}
						if menu_select == status{
							display_set_position(0, 3);
							display_set_color(COLOR_HIGHLIGHT);
							printf("Status");
							display_set_color(COLOR_NORMAL);
						}
						break;	
				}
				if (joystick_get_direction() == JoystickDir_RIGHT){
					menu_current = menu_select;
				}
				break;
			case play_game:
				display_clear();
				display_set_position(0, 0);
				printf("Game simulation:");
				display_set_position(0, 1);
				break;
			case highscore:
				display_clear();
				display_set_position(0, 0);
				printf("Highest score: &d", xmem_get_highscore()); //Reads highest score from memory
				
				if (joystick_get_direction() == JoystickDir_LEFT){
					menu_current = main_menu;
					break;
				}
				display_repaint();
				break;
			case status:				
				display_clear();
				display_set_position(0, 0);
				printf("Status:");
				display_set_position(0, 1);
				if (1){					//CAN OK
				printf("CAN: OK");
				} else {
					printf("CAN: ERROR")
				}
				display_set_position(0, 2);
				if (1){					//Node 2 OK
					printf("Node 2: OK");
					} else {
					printf("Node 2: ERROR")
				}
				display_set_position(0, 3);
				if (1){					//IO OK
					printf("I/O board: OK");
					} else {
					printf("I/O board: ERROR")
				}
				display_set_position(0, 4);
				if (1){					//Encryption OK
					printf("Encryption: OK");
					} else {
					printf("Encryption: ERROR")
				}
				if (joystick_get_direction() == JoystickDir_LEFT){
					menu_current = main_menu;
					break;
				}
				display_repaint();
				break;
		}
		
		CanFrame_t frame = {
			.id = 0x100,
			.length = 0x3,
			.data.u8[0] = joystick_get_x(),
			.data.u8[1] = joystick_get_y(),
			.data.u8[2] = joystick_get_button()
		};

		can_tx_message(&frame);
		frame.data.u8[0] = 0;
		
		if(can_rx_message(&frame)) {
			display_clear();
			display_set_position(0, 0);
			printf("VALUE = %d", frame.data.i16[0]);
			display_set_position(0, 1);
			printf("POS = %d", frame.data.i16[1]);
			
			display_set_position(0, 3);
			printf("SCORE: %d, %d", score, (int)joystick_get_button());


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

			display_repaint();
		}
    }
}

ISR(__vector_default) {
	_delay_ms(1000);
}
