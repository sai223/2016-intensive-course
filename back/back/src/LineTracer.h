/*
 * LineTracer.h
 *
 * Created: 2017. 4. 18. 20:24:46
 *  Author: KimHeeYeon
 */ 


#ifndef LINETRACER_H_
#define LINETRACER_H_


#include "TheArtist.h"

void line_sensor_init(void);
uint8_t line_sensor_read(void);
void line_tracing(void);
void find_line(void);


#endif /* LINETRACER_H_ */