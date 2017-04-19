/*
 * LineTracer.c
 *
 * Created: 2017. 4. 18. 20:18:14
 *  Author: KimHeeYeon
 */ 

#include "TheArtist.h"
#include "LineTracer.h"

void line_sensor_init(void)
{
	struct port_config pin_conf_line[5];
	for(int i = 0; i < 5 ; i++)
	port_get_config_defaults(&pin_conf_line[i]);
	for(int i = 0; i < 5 ; i++){
		pin_conf_line[i].direction = PORT_PIN_DIR_INPUT;
		pin_conf_line[i].input_pull = PORT_PIN_PULL_DOWN;
	}
	
	port_pin_set_config(PIN_PA13, &pin_conf_line[0]); //o1
	port_pin_set_config(PIN_PA28, &pin_conf_line[1]); //o2
	port_pin_set_config(PIN_PA22, &pin_conf_line[2]); //o3
	port_pin_set_config(PIN_PA23, &pin_conf_line[3]); //o4
	port_pin_set_config(PIN_PA15, &pin_conf_line[4]); //o5
	artist_back.line_tracing_run = true;
}

uint8_t line_sensor_read(void){
	uint8_t detect = 0;
	
	detect = port_pin_get_input_level(PIN_PA15); //O5
	detect = detect<<1 | port_pin_get_input_level(PIN_PA23); //O4
	detect = detect<<1 | port_pin_get_input_level(PIN_PA22); //O3
	detect = detect<<1 | port_pin_get_input_level(PIN_PA28); //O2
	detect = detect<<1 | port_pin_get_input_level(PIN_PA13); //O1
	
	return detect;
}

void find_line(void){
	
	artist_motor_backward(&(artist_back.motor_left_side));
	artist_motor_backward(&(artist_back.motor_right_side));
}

void line_tracing(void){
	
	//printf("t");
	uint8_t line_value = line_sensor_read();
	if ((line_value& 0X01110) == CONF_ARTIST_LINE_LOCATED_CENTER_2) {
		artist_motor_forward(&(artist_back.motor_left_side));
		artist_motor_forward(&(artist_back.motor_right_side));
	}
	else if((line_value & CONF_ARTIST_MASKING_LOCATION) == CONF_ARTIST_LINE_LOCATED_LEFT ){ // turn right
		artist_motor_backward(&(artist_back.motor_left_side));
		artist_motor_forward(&(artist_back.motor_right_side));
	}	else if((line_value & CONF_ARTIST_MASKING_LOCATION) == CONF_ARTIST_LINE_LOCATED_RIGHT){ // turn left		
		artist_motor_forward(&(artist_back.motor_left_side));
		artist_motor_backward(&(artist_back.motor_right_side));
	}	else if((line_value & CONF_ARTIST_LINE_LOCATED_CENTER_1) == CONF_ARTIST_LINE_LOCATED_CENTER_1){ // straight
		artist_motor_forward(&(artist_back.motor_left_side)); 
		artist_motor_forward(&(artist_back.motor_right_side));
	}	else
			find_line();
}
