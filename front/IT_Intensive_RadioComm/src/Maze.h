
/*
 * Maze.h
 *
 * Created: 4/8/2017 6:38:59 PM
 *  Author: credt
 */ 

#include "TheArtist.h"

#ifndef __MAZE_H__ 
#define __MAZE_H__ 

#define MAZE_RIGHT_WALL_EXIST_DETERMINATE	5.0
#define MAZE_FRONT_WALL_EXIST_DETERMINATE	5.0
#define MAZE_RIGHT_DISTANCE_LOWERBOUND		5.0
#define MAZE_RIGHT_DISTANCE_UPPERBOUND		7.0

enum DIRECTION_STATUS {
	STRAIGHT, 
	LEFT,
	RIGHT,
	STOP, 
	BACK
};

bool strategic_withdrawal;

void artist_init_maze(void);  
void artist_do_maze (void);  
void artist_pause_maze(void);

#endif 


 