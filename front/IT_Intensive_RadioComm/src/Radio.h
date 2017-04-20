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
#include "sysTimer.h" 

 uint8_t r_data[200][MAX_FRAME_SIZE + 1]; //50  
 uint8_t lnok[MAX_ACK_SIZE];
 uint8_t nack[MAX_ACK_SIZE];
 uint8_t mdok[MAX_ACK_SIZE];
 
 typedef struct Image_frame {
	 
	 uint8_t height;
	 uint8_t width;
 } Image_frame;
 
 typedef enum Radio_state {
	 RECVMODE,
	 RECVFRAME,
	 RECVLINE
 } Radio_state;
 
 Image_frame image_frame;
 Radio_state my_state;
 
 NWK_DataReq_t appDataReq;
 static SYS_Timer_t sendL;
 static SYS_Timer_t sendN;
 static SYS_Timer_t sendM;

 void artist_radio_configure();
 void handle_recvMode(NWK_DataInd_t *ind);
 void handle_recvFrame(NWK_DataInd_t *ind);
 void handle_recvLine(NWK_DataInd_t *ind);
 
 bool receivePKT(NWK_DataInd_t *ind); 
 void radioInit(void);  

 void sendDonePKT(NWK_DataReq_t *req);  
 void sendNACK(void);  
 void sendMDOK(void);
 void sendLNOK(void);

 int receivedLine; //For check num of Received line
 static bool sendBusy;

#endif /*RADIO_H_ */