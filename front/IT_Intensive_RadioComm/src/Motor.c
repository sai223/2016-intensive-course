/*
* Motor.c
*
* Created: 4/24/2017 1:37:18 AM
*  Author: credt
*/
#include <asf.h>
#include "TheArtist.h"
#include "Motor.h"

void artist_draw_motor_pwm_configure(void){
	
	struct tcc_config config;
	
	tcc_get_config_defaults(&config, TCC0);

	config.counter.clock_source = GCLK_GENERATOR_0;
	config.counter.period = 0xFFFF; //CLOCK 's period. when up is occur. it's about 65535
	config.counter.clock_prescaler = 1; // this time we do not divide
	config.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM; // compare

	config.compare.match[CONF_PWM_CHANNEL] = 0x0000;
	config.compare.match[CONF_PWM_CHANNEL2] = 0x0000;
	config.pins.enable_wave_out_pin[CONF_PWM_OUTPUT] = true;
	config.pins.enable_wave_out_pin[CONF_PWM_OUTPUT2] = true;
	config.pins.wave_out_pin[CONF_PWM_OUTPUT] = PIN_PA18F_TCC0_WO2;//PIN_PA19F_TCC0_WO3; // time clock
	config.pins.wave_out_pin_mux[CONF_PWM_OUTPUT] = MUX_PA18F_TCC0_WO2;//MUX_PA19F_TCC0_WO3;
	config.pins.wave_out_pin[CONF_PWM_OUTPUT2] = PIN_PA19F_TCC0_WO3;
	config.pins.wave_out_pin_mux[CONF_PWM_OUTPUT2] = MUX_PA19F_TCC0_WO3;

	tcc_init(&artist_front.tcc_instance, TCC0, &config);
	tcc_enable(&artist_front.tcc_instance);
}

void artist_run_stamp(void) {
	if (artist_front.draw_sequence_counter == 48) {
		usart_write_buffer_job(
		&(artist_front.usart_instance),
		"ls\0\0\0", MAX_RX_BUFFER_LENGTH);
		usart_write_buffer_job(
		&(artist_front.usart_instance),
		"mw\0\0\0", MAX_RX_BUFFER_LENGTH);
		delay_s(2);
		usart_write_buffer_job(
		&(artist_front.usart_instance),
		"ma\0\0\0", MAX_RX_BUFFER_LENGTH);
		delay_s(2);
		usart_write_buffer_job(
		&(artist_front.usart_instance),
		"md\0\0\0", MAX_RX_BUFFER_LENGTH);
		delay_s(2);
		usart_write_buffer_job(
		&(artist_front.usart_instance),
		"m \0\0\0", MAX_RX_BUFFER_LENGTH);
	}
	
	if (real_image_output[artist_front.draw_sequence_counter]) {
		/*
		tcc_set_compare_value(&artist_front.tcc_instance, LED_0_PWM4CTRL_CHANNEL, 0x01ff);
		tcc_set_compare_value(&artist_front.tcc_instance, EXT1_PWM4CTRL_0_CHANNEL, 0x01ff);
		delay_ms(2500);
		tcc_set_compare_value(&artist_front.tcc_instance, LED_0_PWM4CTRL_CHANNEL, 0x431a);
		tcc_set_compare_value(&artist_front.tcc_instance, EXT1_PWM4CTRL_0_CHANNEL, 0x431a);
		delay_ms(400);
		tcc_set_compare_value(&artist_front.tcc_instance, LED_0_PWM4CTRL_CHANNEL, 0x0000);
		tcc_set_compare_value(&artist_front.tcc_instance, EXT1_PWM4CTRL_0_CHANNEL, 0x0000);*/
		delay_ms(3000); 
	}
	else {
		delay_ms(1300);
	}
	artist_front.draw_sequence_counter++;
	
	artist_front.running_stamp = false;
}
