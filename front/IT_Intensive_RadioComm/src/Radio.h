/*
 * radio.h
 *
 * Created: 4/18/2017 9:42:00 PM
 *  Author: credt
 */ 


#ifndef RADIO_H_
#define RADIO_H_

#include "TheArtist.h"
#include "config/conf_radio.h" 

#define MAX_FRAME_SIZE 30
#define ARTIST_FRONT_ADDR 0x0B
#define ARTIST_GROUND_ADDR 0x0A
#define ARTIST_CHANNEL 0x0E

uint8_t r_data[200][MAX_FRAME_SIZE + 1]; //50  
uint8_t t_data[MAX_FRAME_SIZE];

 bool receivePKT(NWK_DataInd_t *ind); 
 void radioInit(void);  

 NWK_DataReq_t appDataReq;
 void sendDonePKT(NWK_DataReq_t *req);  
 void sendPKT(void);  


#endif /* RADIO_H_ */