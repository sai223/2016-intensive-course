#include <asf.h>
#include "TheArtist.h"

void setup() {
	system_init();
	delay_init(); 
	artist_usart_configure(&(artist.usart_instance));
	
	system_interrupt_enable_global(); 
	
	
	// [listen on rx buffer]
	usart_read_buffer_job(&artist.usart_instance,
	(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
	// ! [listen on rx buffer]
	
}

void loop() {
	delay_ms(100); 
}

int main (void)
{
	
	setup (); 

	while(true)	loop();	
	
	// do not come here! 
	
}
