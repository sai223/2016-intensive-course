
/*
 * Motor.h
 *
 * Created: 4/6/2017 8:11:56 PM
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


void artist_draw_motor_pwm_configure(void); 
void artist_run_stamp(void) ;

	
#endif 
