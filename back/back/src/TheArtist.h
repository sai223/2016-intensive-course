
/*
 * TheArtist.h
 *
 * Created: 4/5/2017 2:38:12 PM
 *  Author: credt
 */ 

 #ifndef __THE_ARTIST__
 #define __THE_ARTIST__
#include <asf.h> 

#include "config/conf_the_artist.h"


#define		MAX_RX_BUFFER_LENGTH	5
volatile	uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];


typedef struct Artist{
	struct usart_module usart_instance; 
	struct 
}Artist;

// one instance 
Artist artist;


void setup(); 
void loop(); 

void artist_usart_configure(struct usart_module * usart_instance);

void usart_read_callback(struct usart_module *const usart_module);
void usart_write_callback(struct usart_module *const usart_module);


#endif