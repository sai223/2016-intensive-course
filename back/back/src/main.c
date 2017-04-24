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
	
	artist_draw_motor_tc_configure();
	artist_configure_tc_callbacks();
	
	line_sensor_init();
	artist_status_init();
	system_interrupt_enable_global();
	
	// [SYSTEM INITIALIZE]
	
	printf("b1234");
	
}

void loop() {
	// [listen on RX buffer]
	usart_read_buffer_job(&(artist_back.usart_instance),
	(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
	// ! [listen on RX buffer]
	//printf("%d, %d\n",artist_back.state,artist_back.draw_state );
	if (artist_back.state == DRAW && artist_back.draw_state == TRACING_LINE) {
		artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STOP);
		delay_ms(50);
		line_tracing();
		delay_ms(25);
	}
	else {
		artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STOP);
	}
	
}

int main (void)
{
	setup ();

	while(true)	loop();
	
	// do not come here!
	
}
