
/*
* maze.c
*
* Created: 4/8/2017 6:38:36 PM
*  Author: credt
*/

#include "Maze.h"

void artist_init_maze (void) {
	artist_front.maze_status = STOP;
}

void artist_do_maze (void) {
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
	
	switch (artist_front.maze_status){
		case STRAIGHT :
		usart_write_buffer_job(&(artist_front.usart_instance), "mw\0\0\0", MAX_RX_BUFFER_LENGTH);
		break;
		case STOP :
		usart_write_buffer_job(&(artist_front.usart_instance), "m \0\0\0", MAX_RX_BUFFER_LENGTH);
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
	}
}

void artist_pause_maze(void){
	artist_front.maze_status = STOP;
	usart_write_buffer_job(&(artist_front.usart_instance), "m \0\0\0", MAX_RX_BUFFER_LENGTH);
}