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

uint8_t r_data[60][51]; //50
uint8_t t_data[51];  


 bool receivePKT(NWK_DataInd_t *ind); 
 void radioInit(void);  

 NWK_DataReq_t appDataReq;
 void sendDonePKT(NWK_DataReq_t *req);  
 void sendPKT(void);  


#endif /* RADIO_H_ */