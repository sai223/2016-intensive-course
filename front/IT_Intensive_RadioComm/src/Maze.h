
/*
 * Maze.h
 *
 * Created: 4/8/2017 6:38:59 PM
 *  Author: credt
 */ 

#include "TheArtist.h"

#ifndef __MAZE_H__ 
#define __MAZE_H__ 


enum DIRECTION_STATUS {
	STRAIGHT, 
	LEFT,
	RIGHT,
	STOP, 
	BACK
};
void artist_init_maze(void);  
void artist_do_maze (void);  
#endif 


 