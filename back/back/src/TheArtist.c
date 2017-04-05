
/*
 * TheArtist.c
 *
 * Created: 4/5/2017 2:49:19 PM
 *  Author: credtiger96@gmail.com 
 */ 
#include "TheArtist.h" 
	



void usart_read_callback(struct usart_module * const usart_instance)
{
	
	//port_pin_toggle_output_level(LED_0_PIN);
	switch  (rx_buffer[0])	{
		case 'a' : printf("Hello World!"); 
		port_pin_toggle_output_level(LED_0_PIN);
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
