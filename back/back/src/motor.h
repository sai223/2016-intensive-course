
/*
 * motor.h
 *
 * Created: 4/5/2017 3:17:42 PM
 *  Author: credt
 */ 

#include "TheArtist.h"

#ifndef __MOTOR_H__ 
#define __MOTOR_H__ 

struct artist_motor_module {
	
	uint8_t pwm_pin_num;
	uint8_t pwm_mux_num;
	
	uint16_t pwm_val; 
	
	uint8_t pwm_channel; 
	uint8_t pwm_output; 
	
};	

void artist_motor_configure(struct artist_motor_module * motor_instance, 
				uint8_t pwm_pin, uint8_t pwm_mux,
				uint8_t pwm_channel, uint8_t pwm_output);
void artist_motor_stop(struct artist_motor_module * motor_instance);
void artist_motor_forward(struct artist_motor_module * motor_instance);
void artist_motor_backward(struct artist_motor_module * motor_instance); 

#endif
