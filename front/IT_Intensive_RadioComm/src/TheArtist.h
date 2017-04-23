
/*
 * TheArtist.h
 *
 * Created: 4/6/2017 8:11:14 PM
 *  Author: credt
 */ 

#include <asf.h>

#ifndef __THE_ARTIST__
#define __THE_ARTIST__


#include "config/conf_the_artist_front.h"
#include "phy.h"
#include "nwk.h"
#include "config.h"
#include "sys.h"
#include "Motor.h"
#include "UltraSonic.h"
#include "Maze.h" 
#include "Radio.h"

enum artist_state {
	WAIT,
	DOING_MAZE,
	DRAWING
};

typedef struct Artist_Front{
	struct tc_module				tc_instance_ultrasonic;
	struct tc_module				tc_instance_timer;
	struct tcc_module				tcc_instance;
	struct usart_module				usart_instance; 
	struct artist_motor_module		motor_instance;
	struct artist_ultrasonic_module us_instance_center;
	struct artist_ultrasonic_module us_instance_left;
	struct artist_ultrasonic_module us_instance_right;
	
	float center_distance; 
	float left_distance; 
	float right_distance; 
	

	enum artist_state state; 
	enum DIRECTION_STATUS maze_status;

	
}Artist_Front;


void artist_state_init(void);  
void artist_usart_configure(struct usart_module * usart_instance);
void artist_motor_pwm_configure(struct Artist_Front * const artist); 
void artist_ultrasonic_tc_configure(void);
void artist_scheduler_tc_configure(void); 
void artist_configure_tc_callbacks(void);
void artist_ultrasonic_update(void); 

// one instance
struct Artist_Front artist_front;


#define		MAX_RX_BUFFER_LENGTH	5
volatile	uint8_t rx_buffer[MAX_RX_BUFFER_LENGTH];


#endif