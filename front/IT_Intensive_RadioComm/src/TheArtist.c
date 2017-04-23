
/*
* TheArtist.c
*
* Created: 4/6/2017 8:12:53 PM
*  Author: credt
*/

#include "TheArtist.h"


void artist_state_init() {	
	artist_front.state = WAIT;	
	artist_front.running_stamp = false; 
	artist_front.draw_sequence_counter = 0;  
}

void artist_ultrasonic_tc_configure() {
	
	struct tc_config config;
	
	tc_get_config_defaults(&config);
	
	config.counter_size = TC_COUNTER_SIZE_16BIT;
	config.clock_source = GCLK_GENERATOR_3;
	config.clock_prescaler = TC_CLOCK_PRESCALER_DIV8;
	config.enable_capture_on_channel[TC_COMPARE_CAPTURE_CHANNEL_0] = true;
	tc_init(&(artist_front.tc_instance_ultrasonic), TC4, &config);
	tc_enable(&(artist_front.tc_instance_ultrasonic));
}

void usart_handle_drawing() {
	//printf(rx_buffer); 
		switch(rx_buffer[0]) {
			case 'l' :
			switch (rx_buffer[1]) {
				case 'p': // back node met drawing point
					artist_front.running_stamp = true;
				break;
				case '2':
				break;
			}
			break;
		}
}
void usart_read_callback(struct usart_module * const usart_instance)
{
	//if (artist_front.state == DRAWING) 
	usart_handle_drawing(); 
	
	usart_read_buffer_job( usart_instance,
	(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
}
void usart_write_callback(struct usart_module *const usart_module){}


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

void artist_motor_pwm_configure(struct Artist_Front * const artist){
	
	/* TODO
	struct tcc_config config;
	tcc_get_config_defaults(&config, TCC0);

	config.counter.clock_source											= GCLK_GENERATOR_0;
	config.counter.period												= 0xFFFF; //CLOCK 's period. when up is occur. it's about 65535
	config.counter.clock_prescaler										= 1; // this time we do not divide
	config.compare.wave_generation										= TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM; // compare
	
	
	config.compare.match[artist->motor_instance_1.pwm_channel]				= artist->motor_instance_1.pwm_val;
	config.pins.enable_wave_out_pin[artist->motor_instance_1.pwm_output]	= true;
	config.pins.wave_out_pin[artist->motor_instance_1.pwm_output]			= artist->motor_instance_1.pwm_pin_num;
	config.pins.wave_out_pin_mux[artist->motor_instance_1.pwm_output]		= artist->motor_instance_1.pwm_mux_num;
	
	config.compare.match[artist->motor_insntace_2.pwm_channel]				= artist->motor_insntace_2.pwm_val;
	config.pins.enable_wave_out_pin[artist->motor_insntace_2.pwm_output]		= true;
	config.pins.wave_out_pin[artist->motor_insntace_2.pwm_output]			= artist->motor_insntace_2.pwm_pin_num;
	config.pins.wave_out_pin_mux[artist->motor_insntace_2.pwm_output]		= artist->motor_insntace_2.pwm_mux_num;

	tcc_init(&(artist->tcc_instance), TCC0, &config);
	tcc_enable(&(artist->tcc_instance));
	*/
}

void artist_ultrasonic_update() {
	artist_front.center_distance = artist_ultrasonic_get_value(&(artist_front.us_instance_center));
	artist_front.right_distance = artist_ultrasonic_get_value(&(artist_front.us_instance_right));
	artist_front.left_distance = artist_ultrasonic_get_value(&(artist_front.us_instance_left));
}

void artist_scheduler_tc_configure() {
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
	config_tc.counter_8_bit.period = 50;
	//config_tc.counter_8_bit.compare_capture_channel[0] = 5;
	//config_tc.counter_8_bit.compare_capture_channel[1] = 54;
	//! [setup_change_config]

	//! [20 Hz setup_set_config]
	tc_init(&(artist_front.tc_instance_timer), TC5, &config_tc);
	//! [setup_set_config]

	//! [setup_enable]
	tc_enable(&(artist_front.tc_instance_timer));
	//! [setup_enable]
}


enum artist_state do_state_maze() {
	// [ultra sonic]
	static uint16_t ultrasonic_counter		= 0;
	ultrasonic_counter ++;
	if (ultrasonic_counter > 5) {
		artist_ultrasonic_update();
		ultrasonic_counter = 0;
	}
	
	
	
	static uint16_t maze_counter = 0;
	maze_counter ++;
	if (maze_counter > 25) {
		artist_do_maze();
		maze_counter = 0;
	}
	
	
	#ifdef _ULTRASONIC_DEBUG_
	
	static ultrasonic_counter_debug = 0;
	ultrasonic_counter_debug++;
	if (ultrasonic_counter > 20) {artist_print_ultrasonic_value();}
	
	#endif
	
	// ! [ultra sonic]
	return DOING_MAZE;
}
enum artist_state do_state_drawing() {
	//usart_write_buffer_job(artist_front.usart_instance, "lg\0\0\0", MAX_RX_BUFFER_LENGTH);
	return DRAWING;
}
enum artist_state do_state_wait() {return WAIT; }

void callbacks (void) {
	
	
	switch (artist_front.state) {
		
		case DOING_MAZE:
		artist_front.state = do_state_maze();
		break;
		
		case DRAWING:
		artist_front.state = do_state_drawing();
		break;
		
		case WAIT:
		default :
		artist_front.state = do_state_wait();
	}
}

void artist_configure_tc_callbacks(void)
{
	tc_register_callback(&(artist_front.tc_instance_timer), callbacks ,
	TC_CALLBACK_OVERFLOW);
	
	tc_enable_callback(&(artist_front.tc_instance_timer), TC_CALLBACK_OVERFLOW);
}
