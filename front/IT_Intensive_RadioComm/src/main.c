
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
	artist_draw_motor_pwm_configure(); 
	
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
	
	radioInit();  
	
	printf("f1234");
	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void loop(void) {
	
	usart_read_buffer_job( &(artist_front.usart_instance),
	(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
	
	SYS_TaskHandler();  
		
	
	if (artist_front.running_stamp) {
		
		artist_run_stamp();  
		usart_write_buffer_job(&artist_front.usart_instance, 
		"lg\0\0\0", MAX_RX_BUFFER_LENGTH); 
	}	
	/*
			tcc_set_compare_value(&artist_front.tcc_instance, LED_0_PWM4CTRL_CHANNEL, 0xafff);
			tcc_set_compare_value(&artist_front.tcc_instance, EXT1_PWM4CTRL_0_CHANNEL, 0x431a);
			delay_ms(2500);
			tcc_set_compare_value(&artist_front.tcc_instance, LED_0_PWM4CTRL_CHANNEL, 0x431a);
			tcc_set_compare_value(&artist_front.tcc_instance, EXT1_PWM4CTRL_0_CHANNEL, 0xafff);
			delay_ms(200);
			tcc_set_compare_value(&artist_front.tcc_instance, LED_0_PWM4CTRL_CHANNEL, 0x0000);
			tcc_set_compare_value(&artist_front.tcc_instance, EXT1_PWM4CTRL_0_CHANNEL, 0x0000);
			delay_ms(1000);*/
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int main (void) {
	setup();

	while(true) loop();
}