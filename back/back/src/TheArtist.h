
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

typedef struct Artist{
	struct usart_module usart_instance; 
		
}Artist;

// one instance 
Artist artist;


void setup(); 
void loop(); 

void artist_usart_configure(struct usart_module * usart_instance);

#endif