/*
* Radio.c
*
* Created: 4/18/2017 9:44:15 PM
*  Author: credt
*/
#include "Radio.h"

void artist_radio_configure() {
	strcpy(nack, "NACK\0");
	strcpy(lnok, "LNOK\0");
	strcpy(mdok, "MDOK\0");
	
	receivedLine = 0;
	sendBusy = false;
	my_state = RECVMODE;
}


void artist_drawing_init(void) {
	artist_front.state = DRAWING;
	for (uint8_t i = 0; i < 10; i ++)
	usart_write_buffer_job(
	&(artist_front.usart_instance),
	"lg\0\0\0", MAX_RX_BUFFER_LENGTH);
}


void handle_recvMode(NWK_DataInd_t *ind) {
	//printf("gogo!\n");LED_Toggle(LED0);
	
	if(ind->data[0] == 0x01 && ind->data[1] == 0x02) {
		switch(ind->data[4]) {
			case 0x00 : //Stop
			//printf("STOP (WAIT)");
			if (artist_front.state == DRAWING) {
				for (int i =0 ; i < 50; i ++){
					usart_write_buffer_job(
					&(artist_front.usart_instance),
					"ls\0\0\0", MAX_RX_BUFFER_LENGTH);
					usart_write_buffer_job(
					&(artist_front.usart_instance),
					"m \0\0\0", MAX_RX_BUFFER_LENGTH);
				}
			}
			else {
				for (int i =0 ; i < 50; i ++){
					usart_write_buffer_job(
					&(artist_front.usart_instance),
					"m \0\0\0", MAX_RX_BUFFER_LENGTH);
				}
			}
			artist_front.state = WAIT;
			SYS_TimerStart(&sendM);
			break;
			case 0x01 : //Go forward
			printf("1\n");
			break;
			
			case 0x02 : //Move 360 degree
			break;
			
			case 0x03 : //Data transfer
			my_state = RECVFRAME;
			artist_front.state = WAIT;
			for (int i =0 ; i < 10; i ++)
			usart_write_buffer_job(
			&(artist_front.usart_instance),
			"m \0\0\0", MAX_RX_BUFFER_LENGTH);
			SYS_TimerStart(&sendM);
			break;
			
			case 0x04 :	//Maze
			printf("MAZE MODE\n");
			artist_front.state = DOING_MAZE;
			SYS_TimerStart(&sendM);
			break;
			
			case 0x05 :
			// start drawing
			artist_drawing_init();
			SYS_TimerStart(&sendM);
			break;
			
			default:
			printf("unknowm message (WAIT)");
			artist_front.state = WAIT;
			for (int i =0 ; i < 50; i ++)
			usart_write_buffer_job(
			&(artist_front.usart_instance),
			"m \0\0\0", MAX_RX_BUFFER_LENGTH);
			printf("undefined message\n");
		}
	}
}


void handle_recvFrame(NWK_DataInd_t *ind) {
	
	image_frame.height = ind->data[0];
	image_frame.width = ind->data[1];
	printf("%d %d\n", image_frame.height, image_frame.width);
	
	if(image_frame.height == MAX_FRAME_SIZE || image_frame.width == MAX_FRAME_SIZE) {
		SYS_TimerStart(&sendL);		my_state = RECVLINE;
	}
	
	receivedLine = 0;
}

void handle_recvLine(NWK_DataInd_t *ind) {
	int packet_num = ind->data[0];
	
	for(int i = 0; i<MAX_FRAME_SIZE + 1; i++) {
		r_data[packet_num][i] = ind->data[i];
	}
	
	if(packet_num == receivedLine) {
		SYS_TimerStart(&sendL);
		receivedLine++;
	}
	
	else if(packet_num == receivedLine - 1) {
		SYS_TimerStart(&sendN);
	}
	
	if(receivedLine == image_frame.height) {
		my_state = RECVMODE;
		
		for(int i = 0; i<image_frame.height; i++) {
		for(int j = 0; j<MAX_FRAME_SIZE + 1; j++) {
		printf("%2d", r_data[i][j]);
		}
		printf("\n");
		}
		
		printf("///////////////////////////////\n");
		for (int i =0; i < image_frame.height; i ++ ) {
			for (int j =0; j < image_frame.width; j++ ) {
				if (i % 2) {
					real_image_output[i * image_frame.width + j] = r_data[i][image_frame.width - j];
					}else{
					real_image_output[i * image_frame.width + j] = r_data[i][j + 1];
				}
			}
		}
		for(int i = 0; i<image_frame.height; i++) {
			for(int j = 0; j < image_frame.width ; j++) {
				printf("%2d", real_image_output[i * image_frame.width + j]);
			}
			printf("\n");
		}
	}
}

bool receivePKT(NWK_DataInd_t *ind) {
	//printf("%s", ind->data);
	switch (my_state) {
		case RECVMODE :
		handle_recvMode(ind);
		break;
		
		case RECVFRAME :
		handle_recvFrame(ind);
		break;
		
		case RECVLINE :
		handle_recvLine(ind);
		break;
	}
	return true;
}

void sendDonePKT(NWK_DataReq_t *req) {
	sendBusy = false;
}

void sendLNOK(void) {
	if(sendBusy)
	return;
	
	appDataReq.dstAddr = ARTIST_GROUND_ADDR;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = lnok;
	appDataReq.size = MAX_ACK_SIZE;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	sendBusy = true;
}
void sendNACK(void) {
	if(sendBusy)
	return;
	
	appDataReq.dstAddr = ARTIST_GROUND_ADDR;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = nack;
	appDataReq.size = MAX_ACK_SIZE;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	sendBusy = true;
}
void sendMDOK(void) {
	if(sendBusy)
	return;
	
	appDataReq.dstAddr = ARTIST_GROUND_ADDR;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = mdok;
	appDataReq.size = MAX_ACK_SIZE;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	sendBusy = true;
}

void radioInit(void) {
	artist_radio_configure();
	
	NWK_SetAddr(ARTIST_FRONT_ADDR);  //주소 설정
	NWK_SetPanId(APP_PANID);  //PANID : Personal Area Network ID
	PHY_SetChannel(ARTIST_CHANNEL);
	PHY_SetRxState(true);
	NWK_OpenEndpoint(APP_ENDPOINT, receivePKT);
	
	sendL.interval = 100;
	sendL.mode = SYS_TIMER_INTERVAL_MODE;
	sendL.handler = sendLNOK;
	
	sendN.interval = 100;
	sendN.mode = SYS_TIMER_INTERVAL_MODE;
	sendN.handler = sendNACK;
	
	sendM.interval = 100;
	sendM.mode = SYS_TIMER_INTERVAL_MODE;
	sendM.handler = sendMDOK;
}
