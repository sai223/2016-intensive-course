/*
 * UltraSonic.c
 *
 * Created: 4/6/2017 8:53:00 PM
 *  Author: credtiger96@gmail.com 
 */
#include "TheArtist.h"
#include "UltraSonic.h"

void artist_ultrasonic_configure (struct artist_ultrasonic_module * module, uint8_t echo, uint8_t trig) {
	module->echo_pin	= echo; 
	module->trigger_pin = trig;  
	module->filter_large_change_counts = 0; 
	module->fliter_old_val = 0; 
}

void artist_ultrasonic_gpio_init() {
	struct port_config pin_config_trig;
	struct port_config pin_config_echo_center;
	struct port_config pin_config_echo_left;
	struct port_config pin_config_echo_right; 
	
	port_get_config_defaults(&pin_config_trig);
	port_get_config_defaults(&pin_config_echo_center);
	port_get_config_defaults(&pin_config_echo_right);
	port_get_config_defaults(&pin_config_echo_left);

	pin_config_trig.direction			=	PORT_PIN_DIR_OUTPUT;
	pin_config_trig.input_pull			=	PORT_PIN_PULL_UP;
	
	pin_config_echo_center.direction	=	PORT_PIN_DIR_INPUT;
	pin_config_echo_center.input_pull	=	PORT_PIN_PULL_UP;
	
	pin_config_echo_left.direction		=	PORT_PIN_DIR_INPUT;
	pin_config_echo_left.input_pull		=	PORT_PIN_PULL_UP;
	
	pin_config_echo_right.direction		=	PORT_PIN_DIR_INPUT;
	pin_config_echo_right.input_pull	=	PORT_PIN_PULL_UP;
	
	
	port_pin_set_config(artist_front.us_instance_center.trigger_pin, &pin_config_trig);
	port_pin_set_config(artist_front.us_instance_center.echo_pin, &pin_config_echo_center);
	port_pin_set_config(artist_front.us_instance_left.echo_pin, &pin_config_echo_left);
	port_pin_set_config(artist_front.us_instance_right.echo_pin, &pin_config_echo_right); 
}
static float artist_HC_SR04_filter(struct artist_ultrasonic_module * module,
float new, float old, float FILTER_VAL ) {
	
	if (new > old * module->filter_large_change_counts ||
	new < old * 1/module->filter_large_change_counts ) {
		module->filter_large_change_counts ++;
		if (module->filter_large_change_counts > 3)		
			module->filter_large_change_counts = 0;
		else 											
			new = old;
	}
	if (FILTER_VAL > 1)			FILTER_VAL = 1;
	else if (FILTER_VAL < 0)	FILTER_VAL = 0;

	return (new * (1 - FILTER_VAL)) + (old * (FILTER_VAL));
}

void artist_print_ultrasonic_value() {
	printf("%5.2f, %5.2f, %5.2f\n", 
				artist_front.left_distance, 
				artist_front.center_distance, 
				artist_front.right_distance);  
}
float artist_ultrasonic_get_value(struct artist_ultrasonic_module * const module) {
	// TODO : use timer callback function!
	uint16_t start, end;
	uint16_t count = 0;
	
	port_pin_set_output_level(module->trigger_pin, false);
	delay_us(40);
	port_pin_set_output_level(module->trigger_pin, true);
	delay_us(10);
	port_pin_set_output_level(module->trigger_pin, false);
	tc_start_counter(&(artist_front.tc_instance_ultrasonic)); 
	while (!(port_pin_get_input_level(module->echo_pin))) {
		count ++;
		if (count > 0xFF00) return module->fliter_old_val; 
	}
	count = 0; 
	start = tc_get_count_value(&(artist_front.tc_instance_ultrasonic));
	while ((port_pin_get_input_level(module->echo_pin)))  {
		count ++;
		if (count > 0xFF00) return module->fliter_old_val;  
	}
	end = tc_get_count_value(&(artist_front.tc_instance_ultrasonic));
	tc_stop_counter(&(artist_front.tc_instance_ultrasonic));

	float new = ((float)(end - start)) * 0.017;
	module->fliter_old_val = artist_HC_SR04_filter(module, new, module->fliter_old_val, CONF_ARTIST_ULTRASONIC_FILTERVAL);
	return module->fliter_old_val;
}
