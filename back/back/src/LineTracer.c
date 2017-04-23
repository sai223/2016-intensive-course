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
void handle_lost_line (void) {
	artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STRAIGHT);
}

void line_tracing(void){
	
	uint8_t line_value = line_sensor_read();
	//printf("%4d\n", line_value);
	
	/*
	if (line_value == 0 )
		handle_lost_line(); 	
		
		
	if ((line_value & 0X01110) == CONF_ARTIST_LINE_LOCATED_CENTER_2) {
		artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STRAIGHT);
	}
	else if((line_value & CONF_ARTIST_LINE_LOCATED_CENTER_1) == CONF_ARTIST_LINE_LOCATED_CENTER_1){ // straight
	artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STRAIGHT);
}
	else if((line_value & CONF_ARTIST_MASKING_LOCATION) | CONF_ARTIST_LINE_LOCATED_LEFT ){ // turn right
		artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), RIGHT);
	}	else if((line_value & CONF_ARTIST_MASKING_LOCATION) | CONF_ARTIST_LINE_LOCATED_RIGHT){ // turn left
	artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), LEFT);
	}*/ 
	static int count = 0;  
	static bool is_point = false; 
	if (line_value) {
		is_point = false; 
		if (count > 0 && count < 10) {
			delay_s(2); 
			count = 0; 
		}
	}
	else {
		count++;
		is_point = true;
		if (count > 10) {
			artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), BACK);
			delay_s(2);
			count = 0;  
		}
		
	}
	switch (line_value) {
		case 0x00:  // 00000
		
		artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STRAIGHT);
		
		break;  
		
		case 0x04:	// 00100
		case 0x0E:	// 01110
		case 0x0C: // 01100
		case 0x06: // 00110
		
		artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STRAIGHT);
		
		break; 
		
		case 0x01: // 00001 
		case 0x02: // 00010
		case 0x03: // 00011 
		case 0x07: // 00111
		case 0x0F: // 01111
		
		artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side),LEFT );
		
		break;  
		
		case 0x10: // 10000
		case 0x08: // 01000
		case 0x18: // 11000
		case 0x1C: // 11100
		case 0x1E: // 11110 
		
		artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), RIGHT);
		
		break; 
		
		default :  
		
		artist_move_motor(&(artist_back.motor_left_side), &(artist_back.motor_right_side), STRAIGHT);
		
	}	
}
