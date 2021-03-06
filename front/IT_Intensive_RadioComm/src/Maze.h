/*
 * Maze.h
 *
 * Created: 4/8/2017 6:38:59 PM
 *  Author: credt
 */ 
#include "TheArtist.h"
#ifndef __MAZE_H__ 
#define __MAZE_H__ 

// define constants for solving maze 
#define		MAZE_FRONT_WALL_EXIST_DETERMINATE			16.0
#define		MAZE_FRONT_WALL_LOWERBOUND					5.5
#define		MAZE_RIGHT_DISTANCE_LOWERBOUND				3.5
#define		MAZE_RIGHT_DISTANCE_UPPERBOUND				7.5
#define		MAZE_LEFT_DISTANCE_LOWERBOUND				7.0
#define		MAZE_NOTHING_LOWERBOUND						20.0

// actuator's state
enum DIRECTION_STATUS {
	STRAIGHT, 
	LEFT,
	RIGHT,
	STOP, 
	BACK
};
struct ultrasonic_distances{
	enum DIRECTION_STATUS direction;
	float left_distance;
	float center_distance;
	float right_distance;
}past_distance;


bool strategic_withdrawal;

/************************************************************************/
/* this manage maze's state.*/                                                                     
/************************************************************************/
void artist_init_maze(void);  
void artist_do_maze (void);  
void artist_pause_maze(void);
#endif 
