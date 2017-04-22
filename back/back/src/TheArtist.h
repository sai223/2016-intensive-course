
/*
 * TheArtist.h
 *
 * Created: 4/5/2017 2:38:12 PM
 *  Author: credt
 */
 #include <asf.h>

#ifndef __THE_ARTIST__
#define __THE_ARTIST__


#include "config/conf_the_artist_back.h"
#include "config/conf_line_trace.h"
#include "Motor.h"

#define		MAX_RX_BUFFER_LENGTH	5
volatile	uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];


typedef struct Artist_Back{
	struct tc_module			tc_instance_motor; 
	struct tcc_module			tcc_instance;
	struct usart_module			usart_instance; 
	struct artist_motor_module  motor_left_side;
	struct artist_motor_module  motor_right_side; 
	
	bool line_tracing_run;  
}Artist_Back;

// one instance 


void artist_usart_configure(struct usart_module * usart_instance);
void artist_motor_pwm_configure(struct Artist_Back * const artist); 
void artist_draw_motor_tc_configure(void);
void artist_configure_tc_callbacks(void);


struct Artist_Back artist_back;

#endif