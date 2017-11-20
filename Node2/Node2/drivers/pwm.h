/*
 * pwm.h
 *
 * Created: 02/10/17 11:42:35
 *  Author: Sondre
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>


void pwm_init();
void pwm_set_duty_cycle(uint8_t duty_cycle);
void pwm_set_pulse_width(uint16_t pulse_width_us);
void pwm_set_servo_deflection(int8_t joystick_input);

#endif /* PWM_H_ */