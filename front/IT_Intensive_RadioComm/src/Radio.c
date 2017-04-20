/*
 * Radio.c
 *
 * Created: 4/18/2017 9:44:15 PM
 *  Author: credt
 */ 
#include "Radio.h"   

int line_num = 0;

 bool receivePKT(NWK_DataInd_t *ind) {	 
	 switch(artist_front.state) {
		 
	 }
	return true;
}

 void radioInit(void) {
	NWK_SetAddr(9);  //주소 설정
	NWK_SetPanId(APP_PANID);  //PANID : Personal Area Network ID
	PHY_SetChannel(APP_CHANNEL);
	PHY_SetRxState(true);
	NWK_OpenEndpoint(APP_ENDPOINT, receivePKT);
}
static bool sendBusy= false; // flag 역할 죽을수도 있어서

 void sendDonePKT(NWK_DataReq_t *req) {
	sendBusy = false;
}

 void sendPKT(void) {
	if(sendBusy)
	return;
	
	appDataReq.dstAddr = 10;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = t_data;
	appDataReq.size = 51;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	//LED_Toggle(LED0);
	sendBusy = true;
}
