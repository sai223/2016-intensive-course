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
	
	port_pin_set_config(PIN_PA13, &pin_conf_line[0]);
	port_pin_set_config(PIN_PA28, &pin_conf_line[1]);
	port_pin_set_config(PIN_PA22, &pin_conf_line[2]);
	port_pin_set_config(PIN_PA23, &pin_conf_line[3]);
	port_pin_set_config(PIN_PA15, &pin_conf_line[4]);
}

void line_sensor_read(void){
	
	int detect[5];
	
	detect[0] = port_pin_get_input_level(PIN_PA13);
	detect[1] = port_pin_get_input_level(PIN_PA28);
	detect[2] = port_pin_get_input_level(PIN_PA22);
	detect[3] = port_pin_get_input_level(PIN_PA23);
	detect[4] = port_pin_get_input_level(PIN_PA15);
	
	for(int i = 0; i < 5 ; i++)
		printf("detect[%d] = %d\n", i, detect[i]);
	
	printf("\n");
}