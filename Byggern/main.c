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
#include "drivers/oled.h"
#include "drivers/joystick.h"
#include "drivers/can.h"
#include "drivers/usart.h"

#include "drivers/spi.h"

#include "drivers/mcp2515.h"

#include "game/menu.h"

#include <stdlib.h>

#include "images/img_cat.h"
#include "images/img_wagon.h"
#include "images/img_fireworks.h"


#define LOOP_UPDATE_FREQUENCY_HZ 30
#define TIMER_TARGET_VALUE (F_CPU / (LOOP_UPDATE_FREQUENCY_HZ * 64))

#define unless(x) if(!(x))
#define until(x) while(!(x))

//volatile char* sram = (char*)0x1800;

uint8_t play_sound_command = 0;

int main(void)
{
	xmem_init();

	// Run xmem_test
	usart_init(9600);
	xmem_test();

	spi_init();
	display_init(); // Routs stdout to the display
	display_clear();
	can_init();
	joystick_init();
	joystick_calibrate_center();
	sei();

	// Init timer counter with prescaler /64
	TCCR1B = 0x03; 

	xmem_set_highscore(0);

	{
		CanFrame_t tx_frame = {
			.id = 0x100,
			.length = 4,
			.data.u8[3] = 3
		};
		can_tx_message(&tx_frame);

		for(int i=0;i<101;i++) {
		
			display_set_position(0, 0);
			printf("Loading: %d%%", i);
			display_repaint();
		}
	}

	bool joystick_button_prev = false;

	uint32_t score3x = 0;
	int count_down = 0;
	int forced_shoot_countdown = 0;
	int movie_timer = 0;

	bool game_active = false;
	bool led_broken = false;
	uint8_t led_value = 255;
	uint8_t motor_position = 127;

	// Define menu variables
	enum {
		MENU_NONE = 0,
		MENU_MAIN, 
		MENU_PREPARE_GAME,
		MENU_FIREWORKS,
		MENU_HIGHSCORE, 
		MENU_STATUS,
		MENU_CAT
	} menu_current = MENU_MAIN;
	
    while (1)
    {
	    // Reset loop timer to zero
	    TCNT1 = 0;

		if (!game_active) {
			uint8_t choice = menu_handle_input();

			switch(choice) {
				case 0: 
					menu_current = MENU_PREPARE_GAME; 
					count_down = 6 * LOOP_UPDATE_FREQUENCY_HZ - 1;
					score3x = 0;
					break;
				case 1: menu_current = MENU_HIGHSCORE; break;
				case 2: menu_current = MENU_STATUS; break;
				case 3: menu_current = MENU_CAT; break;
				case 10: menu_current = MENU_MAIN; break;
				default: break;
			}

			// Draw current page
			uint8_t firework_sequence[] = {
				0,1,0,1,2,1,2,3,2,3,4,3,4,5,4,5,6,5,6,
			};

			switch(menu_current) {
				case MENU_MAIN:
					menu_init();
					break;

				case MENU_PREPARE_GAME:
					display_clear();
					display_set_color(COLOR_NORMAL);
					display_set_position(0, 3);
					printf("  Place ball!   ");
					display_set_position(0, 4);
					printf("        %d       ", count_down / LOOP_UPDATE_FREQUENCY_HZ);

					count_down--;

					if (count_down <= LOOP_UPDATE_FREQUENCY_HZ) {
						game_active = true;
						play_sound_command = 2;
						forced_shoot_countdown = 3 * LOOP_UPDATE_FREQUENCY_HZ;
					}
					break;

				case MENU_HIGHSCORE:
					display_clear();
					display_set_color(COLOR_NORMAL);
					display_set_position(0, 0);
					printf("   Highscore    ");
					display_set_position(0, 1);
					printf("   ~~~~~~~~~    ");
					printf("Highest score"); //Reads highest score from memory
					display_set_position(0, 2);
					uint32_t highscore = xmem_get_highscore();
					if (highscore < 100) {
						printf("     > %2lu <     ", highscore);	
					} else {
						printf("     >%4lu<     ", highscore);
					}
				
					break;

				case MENU_STATUS:
					display_clear();
					display_set_color(COLOR_NORMAL);
					display_set_position(0, 0);
					printf("     Status     ");
					display_set_position(0, 1);
					printf("     ~~~~~~     ");
					
					display_set_position(0, 2);
					printf("POS: %d", motor_position);
					
					display_set_position(0, 3);
					printf("LED: %d", led_value);

					display_set_position(0, 4);
					if (led_broken) {					//Encryption OK
						printf("Led broken: YES");
					} else {
						printf("Led broken: NO");
					}
					break;

				case MENU_CAT:
					oled_draw_image(&img_cat, 0, 0);
					break;

				case MENU_FIREWORKS:
					
					for(int i=0;i<sizeof(firework_sequence);i++) {
						oled_draw_image(film_firework[firework_sequence[i]], 0, 0);
						_delay_ms(50);
					}
					menu_current = MENU_HIGHSCORE;
					break;
				default:
					break;
			}
		
			unless (menu_current == MENU_CAT) {
				display_repaint();
			}
		} else {
			// Draw game screen
			display_clear();
			display_set_color(COLOR_NORMAL);
			display_set_position(0, 0);
			printf(" GAME ACTIVATED!");
			display_set_position(0, 1);
			printf(" ~~~~~~~~~~~~~~~");
			display_set_position(0, 3);
			printf("  Score: %lu ", score3x / 3);
			score3x++;
			display_repaint();
			
			int x = joystick_get_x();
			int s = joystick_get_button() ? 0 : 3;
			int i = 1;
			if (x < -40) i = 0;
			if (x > 40) i = 2;
			i += s;

			Image_t* images[] = {
				&img_left_normal,
				&img_forward_normal,
				&img_right_normal,
				&img_left_extend,
				&img_forward_extend,
				&img_right_extend
			};

			oled_draw_image(images[i], (112UL * motor_position) / 255, 6);
		}

		// Read sensors
		int8_t joystick_x = joystick_get_x();
		uint8_t slider = touch_get_slider(TouchSide_RIGHT);
		bool joystick_button = !joystick_get_button();


		// Send CAN message to Node 2
		uint8_t button_message = 0;
		forced_shoot_countdown--;

		if ((joystick_button && !joystick_button_prev) || forced_shoot_countdown <= 0) {
			button_message = 1;
			forced_shoot_countdown = 3 * LOOP_UPDATE_FREQUENCY_HZ;
		}
		joystick_button_prev = joystick_button;
		
		uint8_t position_request = slider;
		uint8_t angle_request = joystick_x;



		// Check end-of-game condition
		if (game_active && led_broken) {
			game_active = false;
			
			play_sound_command = 1;

			if (score3x / 3 > xmem_get_highscore()) {
				xmem_set_highscore(score3x / 3);
				menu_current = MENU_FIREWORKS;
				movie_timer = 0;
			} else {
				menu_current = MENU_MAIN;
			}
		}

		
		CanFrame_t tx_frame = {
			.id = 0x100,
			.length = 0x4,
			.data.u8[0] = game_active ? angle_request : 0,
			.data.u8[1] = game_active ? position_request : 127,
			.data.u8[2] = game_active ? button_message : 0,
			.data.u8[3] = play_sound_command
		};

		can_tx_message(&tx_frame);
		
		CanFrame_t rx_frame;
		// Receive CAN message
		if(can_rx_message(&rx_frame)) {
			led_value = (led_value * 5 + 1 * rx_frame.data.u8[0]) / 6;
			led_broken = (led_value < 0x10);
			motor_position = rx_frame.data.u8[1];
		}

		play_sound_command = 0;

		// Wait for timer to reach value corresponding to 
		// desired update frequency
		while (TCNT1 < TIMER_TARGET_VALUE) { } ;
    }
}

// Default interrupt handler to catch Hard Faults
ISR(__vector_default) {
	_delay_ms(1000);
}
