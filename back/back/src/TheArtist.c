
/*
 * TheArtist.c
 *
 * Created: 4/5/2017 2:49:19 PM
 *  Author: credt
 */ 
#include "TheArtist.h" 


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
	
	stdio_serial_init(usart_instance, EDBG_CDC_MODULE, &config);
}