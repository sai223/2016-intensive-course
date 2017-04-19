/*
 * conf_line_trace.h
 *
 * Created: 4/19/2017 11:50:06 PM
 *  Author: kim hee yeon  
 */ 


#ifndef CONF_LINE_TRACE_H_
#define CONF_LINE_TRACE_H_
#define CONF_ARTIST_MASKING_LOCATION			0x1B		//00011011 ( o5, o4, o3, o2, o1 )
#define CONF_ARTIST_LINE_LOCATED_RIGHT			0x18		//00011000 ( o5, o4 )
#define CONF_ARTIST_LINE_LOCATED_LEFT			0x03		//00000011 ( o2, o1 )
#define CONF_ARTIST_LINE_LOCATED_CENTER_1		0x04		//00000100 ( o3 )
#define CONF_ARTIST_LINE_LOCATED_CENTER_2		0x0A		//00001010 ( o2, o4)
#define CONF_ARTIST_LINE_ESCAPE					0x00		//00000000


#endif /* CONF_LINE_TRACE_H_ */