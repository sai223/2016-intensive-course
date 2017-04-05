#include <asf.h>
#include "TheArtist.h"

void setup() {
	system_init();
	delay_init(); 
	artist_usart_configure(&(artist.usart_instance));
	
}

void loop() {
	
}

int main (void)
{
	
	setup (); 

	while(true)	loop();	
	
	// do not come here! 
	
}
