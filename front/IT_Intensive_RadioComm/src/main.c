
/*
* main.c
*
* Created: 4/6/2017 8:10:25 PM
*  Author: credt
*/

#include "TheArtist.h"
/************************************************************************/
/*                                                                      */
/************************************************************************/
void setup(void) {
	// [SYSTEM INITIALIZE]
	// [NOTICE!]!! sequence of this calls is important!
	irq_initialize_vectors();
	system_init();
	delay_init();
	SYS_Init();	
	
	artist_ultrasonic_tc_configure();
	artist_scheduler_tc_configure();
	artist_configure_tc_callbacks();  
	artist_init_maze(); 

	cpu_irq_enable();
	
	// [ultrasonic]
	artist_ultrasonic_configure(&(artist_front.us_instance_right),
	CONF_ARTIST_RIGHT_ULTRASONIC_ECHO_PIN,
	CONF_ARTIST_ULTRASONIC_TRIGGER_PIN);
	
	artist_ultrasonic_configure(&(artist_front.us_instance_center),
	CONF_ARTIST_CENTER_ULTRASONIC_ECHO_PIN,
	CONF_ARTIST_ULTRASONIC_TRIGGER_PIN);
	
	artist_ultrasonic_configure(&(artist_front.us_instance_left),
	CONF_ARTIST_LEFT_ULTRASONIC_ECHO_PIN,
	CONF_ARTIST_ULTRASONIC_TRIGGER_PIN);
	
	artist_ultrasonic_gpio_init();
	
	//! [ultrasonic]
	
	
	artist_usart_configure(&(artist_front.usart_instance));
	
	system_interrupt_enable_global();

	
	usart_read_buffer_job( &(artist_front.usart_instance),
	(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
	
	//radioInit();  
	
	printf("front node setup complete\n"); 
	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void loop(void) {
	//SYS_TaskHandler();  
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int main (void) {
	setup();

	while(true) loop();
}