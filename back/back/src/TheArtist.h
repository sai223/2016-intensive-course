
/*
 * TheArtist.h
 *
 * Created: 4/5/2017 2:38:12 PM
 *  Author: credt
 */
 #include <asf.h>

#ifndef __THE_ARTIST__
#define __THE_ARTIST__


#include "config/conf_the_artist.h"
#include "Motor.h"

#define		MAX_RX_BUFFER_LENGTH	5
volatile	uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];


typedef struct Artist{
	struct tcc_module			tcc_instance;
	struct usart_module			usart_instance; 
	struct artist_motor_module  motor_instance_1;
	struct artist_motor_module  motor_insntace_2; 
}Artist;

// one instance 


void artist_usart_configure(struct usart_module * usart_instance);
void artist_motor_pwm_configure(struct Artist * const artist); 



struct Artist artist;

#endif