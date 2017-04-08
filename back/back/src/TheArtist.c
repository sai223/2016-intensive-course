
/*
 * TheArtist.c
 *
 * Created: 4/5/2017 2:49:19 PM
 *  Author: credtiger96@gmail.com 
 */ 
#include "TheArtist.h" 
#include "Motor.h"


void usart_read_callback(struct usart_module * const usart_instance)
{
	
	//port_pin_toggle_output_level(LED_0_PIN);
	switch  (rx_buffer[0])	{
		case 'm' : 
		switch (rx_buffer[1]){
			case 'w' :
			artist_motor_forward(&(artist_back.motor_instance_1));
			artist_motor_backward(&(artist_back.motor_insntace_2));
			break;
			case ' ' :
			artist_motor_stop(&(artist_back.motor_instance_1));
			artist_motor_stop(&(artist_back.motor_insntace_2));
			break;
			case 's' :
			artist_motor_backward(&(artist_back.motor_instance_1));
			artist_motor_forward(&(artist_back.motor_insntace_2));
			break;
			case 'a' :
			artist_motor_forward(&(artist_back.motor_instance_1));
			artist_motor_forward(&(artist_back.motor_insntace_2));
			break;
			case 'd' :
			artist_motor_backward(&(artist_back.motor_instance_1));
			artist_motor_backward(&(artist_back.motor_insntace_2));
			break;
		}
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
}