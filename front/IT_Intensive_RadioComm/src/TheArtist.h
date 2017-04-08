
/*
 * TheArtist.h
 *
 * Created: 4/6/2017 8:11:14 PM
 *  Author: credt
 */ 

#include <asf.h>

#ifndef __THE_ARTIST__
#define __THE_ARTIST__


#include "config/conf_the_artist_front.h"
#include "Motor.h"
#include "UltraSonic.h"

#define		MAX_RX_BUFFER_LENGTH	5
volatile	uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];


typedef struct Artist_Front{
	struct tc_module				tc_instance_ultrasonic;
	struct tc_module				tc_instance_timer;
	struct tcc_module				tcc_instance;
	struct usart_module				usart_instance; 
	struct artist_motor_module		motor_instance;
	struct artist_ultrasonic_module us_instance_center;
	struct artist_ultrasonic_module us_instance_left;
	struct artist_ultrasonic_module us_instance_right;
	
	float center_distance; 
	float left_distance; 
	float right_distance; 
	
}Artist_Front;

// one instance 


void artist_usart_configure(struct usart_module * usart_instance);
void artist_motor_pwm_configure(struct Artist_Front * const artist); 
void artist_ultrasonic_tc_configure(void);
void artist_scheduler_tc_configure(void); 
void artist_configure_tc_callbacks(void);
void artist_ultrasonic_update(void); 

struct Artist_Front artist_front;

#endif