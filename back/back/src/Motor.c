
/*
 * Motor.c
 *
 * Created: 4/5/2017 3:18:56 PM
 *  Author: credt
 */ 
#include "Motor.h"

void artist_motor_configure(struct artist_motor_module * motor_instance, uint8_t pwm_pin, uint8_t pwm_mux, 
					uint8_t pwm_channel, uint8_t pwm_output) {
	 motor_instance->pwm_pin_num	= pwm_pin;
	 motor_instance->pwm_mux_num	= pwm_mux;
	 motor_instance->pwm_val		= CONF_ARTIST_MOTOR_PWM_STOP; 
	 motor_instance->pwm_channel	= pwm_channel; 
	 motor_instance->pwm_output		= pwm_output; 
 }

void artist_move_motor(struct artist_motor_module * motor_left_side,
					struct artist_motor_module * motor_right_side,
					enum DIRECTION_STATUS move_direction) {
	if(move_direction == STRAIGHT){
		artist_motor_forward(motor_left_side);
		artist_motor_backward(motor_right_side);
	}
	else if(move_direction == LEFT){
		artist_motor_forward(motor_left_side);
		artist_motor_forward(motor_right_side);
	}
	else if(move_direction == RIGHT){
		artist_motor_backward(motor_left_side);
		artist_motor_backward(motor_right_side);
	}
	else if(move_direction == BACK){
		artist_motor_backward(motor_left_side);
		artist_motor_forward(motor_right_side);
	}
	else if(move_direction == STOP){
		artist_motor_stop(motor_left_side);
		artist_motor_stop(motor_right_side);
	}
}

void artist_motor_stop(struct artist_motor_module * motor_instance) {
	motor_instance->pwm_val = CONF_ARTIST_MOTOR_PWM_STOP; 
	tcc_set_compare_value(&(artist_back.tcc_instance), motor_instance->pwm_channel, CONF_ARTIST_MOTOR_PWM_STOP);
}
void artist_motor_forward(struct artist_motor_module * motor_instance){
	motor_instance->pwm_val = CONF_ARTIST_MOTOR_PWM_FORWORD;
	tcc_set_compare_value(&(artist_back.tcc_instance), motor_instance->pwm_channel, CONF_ARTIST_MOTOR_PWM_FORWORD);
}
void artist_motor_backward(struct artist_motor_module * motor_instance){
	motor_instance->pwm_val = CONF_ARTIST_MOTOR_PWM_FORWORD;
	tcc_set_compare_value(&(artist_back.tcc_instance), motor_instance->pwm_channel, CONF_ARTIST_MOTOR_PWM_BACKWORD);
}