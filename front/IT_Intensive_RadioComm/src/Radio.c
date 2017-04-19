/*
 * Radio.c
 *
 * Created: 4/18/2017 9:44:15 PM
 *  Author: credt
 */ 
#include "Radio.h"   

int line_num = 0;

 bool receivePKT(NWK_DataInd_t *ind) {
	//printf("receivePKT %d\n", line_num);
	/*  
	printf("\n");
	for(int i = 0; i<51;i++) {
		r_data[line_num][i] = ind->data[i];
		printf("%d ", r_data[line_num][i]);
	}

	LED_Toggle(LED0);
	line_num++;
	*/  
	//printf("%s", ind->data);   
	return true;
}

 void radioInit(void) {
	NWK_SetAddr(9);  //�ּ� ����
	NWK_SetPanId(APP_PANID);  //PANID : Personal Area Network ID
	PHY_SetChannel(APP_CHANNEL);
	PHY_SetRxState(true);
	NWK_OpenEndpoint(APP_ENDPOINT, receivePKT);
}
static bool sendBusy= false; // flag ���� �������� �־

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