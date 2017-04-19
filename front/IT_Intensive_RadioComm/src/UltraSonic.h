/*
 * UltraSonic.h
 *
 
 * Created: 4/6/2017 8:52:17 PM
 *  Author: credtiger96@gmail.com
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#define ULTRASONIC_DEBUG_


 // configure
#include <asf.h>

struct artist_ultrasonic_module { 
	uint8_t echo_pin;
	uint8_t trigger_pin; 
	
	uint8_t filter_large_change_counts; 
	float fliter_old_val; 
}; 

void	artist_ultrasonic_configure (struct artist_ultrasonic_module * module, 
						uint8_t echo, uint8_t trig);  
void	artist_ultrasonic_gpio_init(); 
float	artist_ultrasonic_get_value(struct artist_ultrasonic_module * const module); 
void	artist_print_ultrasonic_value();

#endif /* ULTRASONIC_H_ */