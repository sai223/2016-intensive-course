/*
 * Radio.c
 *
 * Created: 4/18/2017 9:44:15 PM
 *  Author: credt
 */ 
#include "Radio.h"   

typedef struct Image_frame {
	
	uint8_t height;
	uint8_t width;
} Image_frame;

typedef enum radio_state {
	RECVMODE, 
	RECVFRAME,
	RECVLINE
} radio_state;

radio_state my_state = RECVMODE;
Image_frame image_frame;

int line_num = 0;
bool receivePKT(NWK_DataInd_t *ind) {
	LED_Toggle(LED0);
	
	switch (my_state) {
		case RECVMODE :
			if(ind->data[0] == 0x01 && ind->data[1] == 0x02 && ind->data[4] == 0x00) {
				//Stop
				break;
			}		
		
			else if(ind->data[0] == 0x01 && ind->data[1] == 0x02 && ind->data[4] == 0x01) {
				//Go forward
				break;
			}
			
			else if(ind->data[0] == 0x01 && ind->data[1] == 0x02 && ind->data[4] == 0x02) {
				//Move 360 degree
				break;
			}
			
			else if(ind->data[0] == 0x01 && ind->data[1] == 0x02 && ind->data[4] == 0x03) {
				//Draw Mode
				printf("DRAW MODE\n");
				my_state = RECVFRAME;
				break;
			}
		
			else if(ind->data[0] == 0x01 && ind->data[1] == 0x02 && ind->data[4] == 0x04) {
				//Maze Mode
				printf("MAZE MODE\n");
				break;
			}
			
			else {
				//Ignore
				break;
			}
			
		case RECVFRAME :
			image_frame.height = ind->data[0];
			image_frame.width = ind->data[1];
			my_state = RECVLINE;
			printf("%d %d\n", image_frame.height, image_frame.width);			
			break;
			
		case RECVLINE :
			for(int i = 0; i<MAX_FRAME_SIZE+1; i++) {
				r_data[line_num][i] = ind->data[i];
			}
			for(int i = 0; i<MAX_FRAME_SIZE + 1; i++) {
				printf("%d ", r_data[line_num][i]);
			}
			printf("\n");
			line_num++;
			if(line_num == image_frame.height) {
				line_num = 0;
				my_state = RECVMODE;
				/*
				for(int i = 0; i<image_frame.height; i++) {
					for(int j = 0; j<MAX_FRAME_SIZE + 1; j++) {
						printf("%d ", r_data[i][j]);
					}	
					printf("\n");
				}
				*/
			}
			break;
	}
	
	return true;
}

void radioInit(void) {
	NWK_SetAddr(ARTIST_FRONT_ADDR);  //주소 설정
	NWK_SetPanId(APP_PANID);  //PANID : Personal Area Network ID
	PHY_SetChannel(ARTIST_CHANNEL);
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
	
	appDataReq.dstAddr = ARTIST_GROUND_ADDR;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = t_data;
	appDataReq.size = 30;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	sendBusy = true;
}
