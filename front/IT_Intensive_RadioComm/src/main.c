
/*
* main.c
*
* Created: 4/6/2017 8:10:25 PM
*  Author: credt
*/

#include "phy.h"
#include "nwk.h"
#include "config.h"
#include "sys.h"
#include "sio2host.h"
#include "TheArtist.h"
/************************************************************************/
/*                                                                      */
/************************************************************************/
void setup(void) {
	// [SYSTEM INITIALIZE]
	// [NOTICE!]!! sequence of this calls is important!
	irq_initialize_vectors();
	system_init();
	artist_usart_configure(&(artist_front.usart_instance));
	artist_ultrasonic_tc_configure();
	artist_scheduler_tc_configure(); 
	artist_configure_tc_callbacks(); 
	
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
	
	
	delay_init();
	SYS_Init();
	sio2host_init();
	cpu_irq_enable();
	// ![SYSTEM INITIALIZE]
	
	printf("Front board setup complete.\n"); 
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void loop(void) {
	delay_ms(200);
	
	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int main (void) {
	setup();
	while(true) loop();
}