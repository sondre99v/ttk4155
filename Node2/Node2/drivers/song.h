/*
 * song.h
 *
 * Created: 20/11/17 11:21:05
 *  Author: Sondre
 */ 


#ifndef SONG_H_
#define SONG_H_

typedef enum {
	SONG_END,
	SONG_START,
	SONG_LOADING,
	SONG_BEEP
} SoundSong_t;


void song_play(SoundSong_t song);


#endif /* SONG_H_ */