
/*
* TheArtist.c
*
* Created: 4/5/2017 2:49:19 PM
*  Author: credtiger96@gmail.com
*/
#include "TheArtist.h"
#include "Motor.h"

void artist_drawing_go() {
	artist_back.state = DRAW;
	artist_back.draw_state = TRACING_LINE;
}
void artist_drawing_stop() {
	artist_back.state = STOP;
	artist_back.draw_state = WAIT;
	artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STOP);
	artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STOP);
}
void artist_status_init() {
	//printf("state_init : draw\n");
	artist_back.state = STOP;
	artist_back.draw_state = WAIT;
}

void usart_read_callback(struct usart_module * const usart_instance)
{
	//port_pin_toggle_output_level(LED_0_PIN);
	switch  (rx_buffer[0])	{
		case 'm' :
		switch (rx_buffer[1]){
			case 'w' :
			artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STRAIGHT);
			break;
			case 'a' :
			artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), LEFT);
			break;
			case 'd' :
			artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), RIGHT);
			break;
			case 's' :
			artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), BACK);
			break;
			case ' ' :
			artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STOP);
			break;
		}
		break;
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		case 'l':
		switch(rx_buffer[1]) {
			case 's' :  // stop
			artist_drawing_stop();
			break;
			
			case 'g' :  //go or stamping complete. 
			artist_drawing_go();
			break;
			
			case '4' : // drawing complete.
			printf("STMPC");
		}
		break;
	}
	usart_read_buffer_job( usart_instance,
	(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
}
void usart_write_callback(struct usart_module *const usart_module)
{
	//port_pin_toggle_output_level(LED_0_PIN);
}


void configure_usart_callbacks(struct usart_module * usart_instance)
{
	//! [setup_register_callbacks]
	usart_register_callback(usart_instance,
	usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	
	usart_register_callback(usart_instance,
	usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	//! [setup_register_callbacks]

	//! [setup_enable_callbacks]
	usart_enable_callback(usart_instance, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
	//! [setup_enable_callbacks]
}

void artist_usart_configure(struct usart_module * usart_instance) {
	struct usart_config config;
	usart_get_config_defaults(&config);
	config.baudrate = CONF_ARTIST_BAUDRATE;
	config.mux_setting	= EDBG_CDC_SERCOM_MUX_SETTING;
	config.pinmux_pad0	= EDBG_CDC_SERCOM_PINMUX_PAD0;
	config.pinmux_pad1	= EDBG_CDC_SERCOM_PINMUX_PAD1;
	config.pinmux_pad2	= EDBG_CDC_SERCOM_PINMUX_PAD2;
	config.pinmux_pad3	= EDBG_CDC_SERCOM_PINMUX_PAD3;
	while (usart_init(usart_instance, EDBG_CDC_MODULE, &config) != STATUS_OK);
	
	usart_enable(usart_instance);
	configure_usart_callbacks(usart_instance);
	
	stdio_serial_init(usart_instance, EDBG_CDC_MODULE, &config);
}


void artist_motor_pwm_configure(struct Artist_Back * const artist){
	
	struct tcc_config config;
	tcc_get_config_defaults(&config, TCC0);

	config.counter.clock_source											= GCLK_GENERATOR_0;
	config.counter.period												= 0xFFFF; //CLOCK 's period. when up is occur. it's about 65535
	config.counter.clock_prescaler										= 1; // this time we do not divide
	config.compare.wave_generation										= TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM; // compare
	
	
	config.compare.match[artist->motor_left_side.pwm_channel]				= artist->motor_left_side.pwm_val;
	config.pins.enable_wave_out_pin[artist->motor_left_side.pwm_output]		=	true;
	config.pins.wave_out_pin[artist->motor_left_side.pwm_output]			= artist->motor_left_side.pwm_pin_num;
	config.pins.wave_out_pin_mux[artist->motor_left_side.pwm_output]		= artist->motor_left_side.pwm_mux_num;
	
	config.compare.match[artist->motor_right_side.pwm_channel]				= artist->motor_right_side.pwm_val;
	config.pins.enable_wave_out_pin[artist->motor_right_side.pwm_output]		= true;
	config.pins.wave_out_pin[artist->motor_right_side.pwm_output]			= artist->motor_right_side.pwm_pin_num;
	config.pins.wave_out_pin_mux[artist->motor_right_side.pwm_output]		= artist->motor_right_side.pwm_mux_num;

	tcc_init(&(artist->tcc_instance), TCC0, &config);
	tcc_enable(&(artist->tcc_instance));
}
void artist_draw_motor_tc_configure(void) {
	//! [setup_config]
	struct tc_config config_tc;
	//! [setup_config]
	//! [setup_config_defaults]
	tc_get_config_defaults(&config_tc);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_tc.counter_size = TC_COUNTER_SIZE_8BIT;
	config_tc.clock_source = GCLK_GENERATOR_3;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1024; //
	config_tc.counter_8_bit.period = 20;
	//config_tc.counter_8_bit.compare_capture_channel[0] = 5;
	//config_tc.counter_8_bit.compare_capture_channel[1] = 54;
	//! [setup_change_config]

	//! [20 Hz setup_set_config]
	tc_init(&(artist_back.tc_instance_motor), TC4, &config_tc);
	//! [setup_set_config]

	//! [setup_enable]
	tc_enable(&(artist_back.tc_instance_motor));
	//! [setup_enable]
	
}
void do_drawing() {
	line_tracing();
};
void callbacks (void) {
	switch (artist_back.state) {
		case WAIT :
		break;
		case DRAW :
		switch (artist_back.draw_state) {
			case TRACING_LINE :
			//do_drawing();
			break;
			
		}
		break;
	}
}

void artist_configure_tc_callbacks(void)
{
	tc_register_callback(&(artist_back.tc_instance_motor), callbacks ,
	TC_CALLBACK_OVERFLOW);
	
	tc_enable_callback(&(artist_back.tc_instance_motor), TC_CALLBACK_OVERFLOW);
}