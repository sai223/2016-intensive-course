#include <asf.h>
#include "TheArtist.h"

void setup() {
	system_init();
	delay_init(); 
	artist_usart_configure(&(artist.usart_instance)); 

}

void loop() {
	delay_ms(1000); 
	printf("Hello world!"); 
}

int main (void)
{
	
	setup (); 
	
	
	while(true)	loop();	
}
