#include <asf.h>
#include "TheArtist.h"

void setup() {
	// [SYSTEM INITIALIZE]
	system_init();
	delay_init(); 
	artist_usart_configure(&(artist_back.usart_instance));
	artist_motor_configure(&(artist_back.motor_left_side),
				CONF_ARTIST_MOTOR_1_PWM_PIN, CONF_ARTIST_MOTOR_1_PWM_MUX,
				CONF_ARTIST_MOTOR_1_PWM_CHANNEL, CONF_ARTIST_MOTOR_1_PWM_OUTPUT);
	artist_motor_configure(&(artist_back.motor_right_side),
				CONF_ARTIST_MOTOR_2_PWM_PIN, CONF_ARTIST_MOTOR_2_PWM_MUX ,
				CONF_ARTIST_MOTOR_2_PWM_CHANNEL, CONF_ARTIST_MOTOR_2_PWM_OUTPUT); 
	artist_motor_pwm_configure(&artist_back);
	
	system_interrupt_enable_global(); 
	
	// [SYSTEM INITIALIZE] 

	// [listen on RX buffer]
	while(true){
	usart_read_buffer_job(&(artist_back.usart_instance),
	(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
	}
	// ! [listen on RX buffer]
	
	printf("back node setup complete\n"); 
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
