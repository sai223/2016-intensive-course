
/*
* maze.c
*
* Created: 4/8/2017 6:38:36 PM
*  Author: credtiger96, Acka, Kyle  
*/
#include "Maze.h"
void artist_init_maze (void) {
	artist_front.maze_status = STOP;
	past_distance.direction = STOP;
}
/*
void legacy_maze1(void){
	if(artist_front.center_distance <= MAZE_FRONT_WALL_EXIST_DETERMINATE){
		strategic_withdrawal = true;
		artist_front.maze_status = BACK;
	}
	else if(artist_front.right_distance < MAZE_RIGHT_DISTANCE_LOWERBOUND){
		artist_front.maze_status = LEFT;
	}
	else if(artist_front.right_distance > MAZE_RIGHT_DISTANCE_UPPERBOUND){
		artist_front.maze_status = RIGHT;
	}
	else{
		if(strategic_withdrawal){
			strategic_withdrawal = false;
			artist_front.maze_status = LEFT;
		}
		else artist_front.maze_status = STRAIGHT;
	}
	
	artist_print_ultrasonic_value();
	if(artist_front.maze_status == STRAIGHT) printf("%s\n", "STRA\0");
	else if(artist_front.maze_status == LEFT) printf("%s\n", "LEFT\0");
	else if(artist_front.maze_status == RIGHT) printf("%s\n", "RIGH\0");
	else printf("%s\n", "BACK\0");
}
*/
void artist_do_maze (void) {
	if(past_distance.direction == RIGHT || past_distance.direction == LEFT){
		artist_front.maze_status = STRAIGHT;
	}
	else if(artist_front.center_distance < MAZE_FRONT_WALL_EXIST_DETERMINATE){
		artist_front.maze_status = LEFT;
	}
	else if(artist_front.left_distance < MAZE_LEFT_DISTANCE_LOWERBOUND
		&& artist_front.right_distance < MAZE_RIGHT_DISTANCE_UPPERBOUND){
		artist_front.maze_status = STRAIGHT;		
	}
	else if(artist_front.left_distance > MAZE_NOTHING_LOWERBOUND 
		&& artist_front.right_distance > MAZE_NOTHING_LOWERBOUND){
		artist_front.maze_status = STRAIGHT;
	}
	else if(artist_front.left_distance < MAZE_LEFT_DISTANCE_LOWERBOUND){
		artist_front.maze_status = RIGHT;
	}
	else if(artist_front.right_distance > MAZE_RIGHT_DISTANCE_UPPERBOUND){
		artist_front.maze_status = RIGHT;
	}
	else if(artist_front.right_distance < MAZE_RIGHT_DISTANCE_LOWERBOUND){
		artist_front.maze_status = LEFT;
	}
	else{
		artist_front.maze_status = STRAIGHT;
	}
	
	past_distance.direction = artist_front.maze_status;
	past_distance.left_distance = artist_front.left_distance;
	past_distance.center_distance = artist_front.center_distance;
	past_distance.right_distance = artist_front.right_distance;
	
	#ifdef MAZE_DEBUG
	
	artist_print_ultrasonic_value();
	if(artist_front.maze_status == STRAIGHT) printf("%s\n", "STRA\0");
	else if(artist_front.maze_status == LEFT) printf("%s\n", "LEFT\0");
	else if(artist_front.maze_status == RIGHT) printf("%s\n", "RIGH\0");
	else printf("%s\n", "BACK\0");
	
	#endif
	
	switch (artist_front.maze_status){
		case STRAIGHT :
		usart_write_buffer_job(&(artist_front.usart_instance), "mw\0\0\0", MAX_RX_BUFFER_LENGTH);
		break;
		case LEFT :
		usart_write_buffer_job(&(artist_front.usart_instance), "ma\0\0\0", MAX_RX_BUFFER_LENGTH);
		break;
		case RIGHT :
		usart_write_buffer_job(&(artist_front.usart_instance), "md\0\0\0", MAX_RX_BUFFER_LENGTH);
		break;
		case BACK :
		usart_write_buffer_job(&(artist_front.usart_instance), "ms\0\0\0", MAX_RX_BUFFER_LENGTH);
		break;
		case STOP :
		usart_write_buffer_job(&(artist_front.usart_instance), "m \0\0\0", MAX_RX_BUFFER_LENGTH);
		break;
	}
}
void artist_pause_maze(void){
	artist_front.maze_status = STOP;
	usart_write_buffer_job(&(artist_front.usart_instance), "m \0\0\0", MAX_RX_BUFFER_LENGTH);
}