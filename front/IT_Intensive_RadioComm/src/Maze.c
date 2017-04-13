
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
	if (artist_front.center_distance < 12) {
		if (artist_front.left_distance < 12 && artist_front.right_distance < 12) { 
			artist_front.maze_status = BACK; 
		}
		else if  (artist_front.left_distance < artist_front.right_distance) {
			
			artist_front.maze_status = RIGHT;
		}
		else {
			artist_front.maze_status = LEFT;
		}
		}else {
		artist_front.maze_status = STRIGHT;
	}
	
	switch (artist_front.maze_status){
		case STRIGHT :
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
