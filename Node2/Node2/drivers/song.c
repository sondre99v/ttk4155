/*
 * song.c
 *
 * Created: 30/10/17 14:32:05
 *  Author: Sondre
 */ 


#include <util/delay.h>
#include "motor.h"

#include "song.h"


void delay_var_us(int us) {
	for(volatile int i = 0; i < us; i++) {
		_delay_us(1);
	}
}

const int motor_amplitude = 200;


void buzz(long frequency, long length) {
	volatile long delayValue = 1000000 / frequency / 2;
	volatile long numCycles = frequency * length / 1000;

	for (long i = 0; i < numCycles; i++) {
		motor_set((frequency != -1) ? motor_amplitude : 0, DIR_LEFT);
		delay_var_us(delayValue);
		motor_set((frequency != -1) ? motor_amplitude : 0, DIR_RIGHT);
		delay_var_us(delayValue);
	}
	
	motor_set(0, DIR_RIGHT);
}

void song_play(SoundSong_t song) {
	uint32_t i;
	switch(song) {
		case SONG_END:
			buzz(1000, 120);
			buzz(800, 120);
			break;
		case SONG_START:
			buzz(2000, 50);
			buzz(3000, 50);
			buzz(4000, 50);
			break;
		case SONG_LOADING:
			for(i = 0; i < 120; i++) {
				buzz(i * i * i / 120, 6);
			}
			break;
	}
}
