#include "asf.h"
#include "phy.h"
#include "nwk.h"
#include "config.h"
#include "sys.h"
#include "sio2host.h"
#include <string.h>

uint8_t r_data[50][51];
uint8_t t_data[51];

int line_num = 0;
static bool receivePKT(NWK_DataInd_t *ind) {
	//printf("receivePKT %d\n", line_num);
	printf("\n");
	for(int i = 0; i<51;i++) {
		r_data[line_num][i] = ind->data[i];
		printf("%d", r_data[line_num][i]);
	}

	LED_Toggle(LED0);
	line_num++;
	return true;
}

static void radioInit(void) {
	NWK_SetAddr(9);  //주소 설정
	NWK_SetPanId(APP_PANID);  //PANID : Personal Area Network ID
	PHY_SetChannel(APP_CHANNEL);
	PHY_SetRxState(true);
	NWK_OpenEndpoint(APP_ENDPOINT, receivePKT);
}

static NWK_DataReq_t appDataReq;
//static uint8_t data[] = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
static bool sendBusy= false; // flag 역할 죽을수도 있어서

static void sendDonePKT(NWK_DataReq_t *req) {
	sendBusy = false;
}

static void sendPKT(void) {
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

int main () {
	irq_initialize_vectors();
	system_init();
	delay_init();
	SYS_Init();
	sio2host_init();
	cpu_irq_enable();

	//for(int i = 0; i < 50; i++) {
	//	memset(r_data[i],8,51*sizeof(uint8_t));
	//}


	radioInit();
	
	//sendPKT();
	
	while (1) {
		SYS_TaskHandler();
	}
}