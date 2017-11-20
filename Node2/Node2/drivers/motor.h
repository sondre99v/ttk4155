/*
 * motor.h
 *
 * Created: 09/10/17 09:50:16
 *  Author: Sondre
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_


#include <stdint.h>


typedef enum {
	DIR_LEFT,
	DIR_RIGHT
} motor_direction_t;


void motor_init();
void motor_enable();
void motor_disable();
void motor_set(uint8_t speed, motor_direction_t direction);
void motor_calibrate_position();

void motor_regulate(float encoder_position, float slider);

void motor_reset_encoder();
float motor_get_position();


#endif /* MOTOR_H_ */