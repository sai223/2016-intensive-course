
#include <asf.h>
#include <string.h>

#include "asf.h"
#include "phy.h"
#include "nwk.h"
#include "config.h"
#include "sys.h"
#include "systimer.h"

static SYS_Timer_t sendT;
static SYS_Timer_t sendM;

struct usart_module usart_instance;

static NWK_DataReq_t appDataReq;

bool sendBusy= false;

#define MAX_FRAME_SIZE 8
#define MAX_ARTISTMODE_SIZE 5
#define MAX_ACK_SIZE 5

#define ARTIST_GROUND_ADDR 0x0A
#define ARTIST_FRONT_ADDR 0x0B
#define ARTIST_CHANNEL 0x0E

typedef struct Image_frame {
	
	uint8_t height;
	uint8_t width;
}Image_frame;
Image_frame image_frame;

uint8_t artistMode[MAX_ARTISTMODE_SIZE];

uint8_t r_data[MAX_FRAME_SIZE + 1][MAX_FRAME_SIZE + 1];

uint8_t ack[MAX_ACK_SIZE] = "okay\0";


void configure_usart(void)
{
	struct usart_config config_usart;
	usart_get_config_defaults(&config_usart);
	config_usart.baudrate		= 9600;
	config_usart.mux_setting	= EDBG_CDC_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0	= EDBG_CDC_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1	= EDBG_CDC_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2	= EDBG_CDC_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3	= EDBG_CDC_SERCOM_PINMUX_PAD3;

	while(usart_init(&usart_instance, EDBG_CDC_MODULE, &config_usart) != STATUS_OK) {
	}

	usart_enable(&usart_instance);

	stdio_serial_init(&usart_instance, EDBG_CDC_MODULE, &config_usart);
	
	usart_enable_transceiver(&usart_instance, USART_TRANSCEIVER_TX);
	usart_enable_transceiver(&usart_instance, USART_TRANSCEIVER_RX);
}


int line_count = 0;
static bool receivePKT(NWK_DataInd_t *ind) {
	LED_Toggle(LED0);
	printf("%s\n", ind->data);
	
	if(!strcmp(ind->data, "LNOK\0")) {
		line_count++;
		if(line_count == image_frame.height + 1) {
			SYS_TimerStop(&sendT);
			line_count = 0;
		}
	}
	
	else if(!strcmp(ind->data, "NACK\0")) {
		line_count++;		
	}
	
	else if(!strcmp(ind->data, "MDOK\0")) {
		
	}
	

	return true;
}

static void sendDonePKT(NWK_DataReq_t *req) {
	sendBusy = false;
}

static void sendArtistDonePKT(NWK_DataReq_t *req) {
	sendBusy = false;
}

static void sendArtistMode(void) {
	
	if(sendBusy)
	return;
	
	appDataReq.dstAddr = ARTIST_FRONT_ADDR;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = artistMode;
	appDataReq.size = MAX_ARTISTMODE_SIZE;
	appDataReq.confirm = sendArtistDonePKT;
	NWK_DataReq(&appDataReq);
	
	sendBusy = true;
}


static void sendArtistPKT(void) {
	if(sendBusy)
	return;
	
	appDataReq.dstAddr = ARTIST_FRONT_ADDR;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = r_data[line_count];
	appDataReq.size = MAX_FRAME_SIZE + 1;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	sendBusy = true;
	printf("%d complete\n", line_count);
}

static void radioInit(void) {
	NWK_SetAddr(ARTIST_GROUND_ADDR);  
	NWK_SetPanId(APP_PANID);  
	PHY_SetChannel(ARTIST_CHANNEL);
	PHY_SetRxState(true);
	NWK_OpenEndpoint(APP_ENDPOINT, receivePKT);
	
	sendT.interval = 400; 
	sendT.mode =SYS_TIMER_PERIODIC_MODE;
	sendT.handler = sendArtistPKT;
	
	sendM.interval = 200;
	sendM.mode = SYS_TIMER_INTERVAL_MODE;
	sendM.handler = sendArtistMode;
}

void receive_mode () {
	
	uint8_t mode[5] = {0,};
	
	while(true) {
		SYS_TaskHandler();
		if (usart_read_buffer_wait(&usart_instance, mode, sizeof(mode)) == STATUS_OK) {
			if( !strcmp(mode, "draw\0") || !strcmp(mode, "maze\0") || !strcmp(mode, "wait\0") || !strcmp(mode, "data\0") ) {
				LED_Toggle(LED0);
				
				if(!strcmp(mode, "wait\0")) {
					artistMode[0] = 0x01;
					artistMode[1] = 0x02;
					artistMode[2] = 0xFF;
					artistMode[3] = 0xFF;
					artistMode[4] = 0x00;
				}
				if(!strcmp(mode, "data\0")) {
					artistMode[0] = 0x01;
					artistMode[1] = 0x02;
					artistMode[2] = 0xFF;
					artistMode[3] = 0xFF;
					artistMode[4] = 0x03;
				}			
				if(!strcmp(mode, "maze\0")) {
					artistMode[0] = 0x01;
					artistMode[1] = 0x02;
					artistMode[2] = 0xFF;
					artistMode[3] = 0xFF;
					artistMode[4] = 0x04;
				}					
				if(!strcmp(mode, "draw\0")) {
					artistMode[0] = 0x01;
					artistMode[1] = 0x02;
					artistMode[2] = 0xFF;
					artistMode[3] = 0xFF;
					artistMode[4] = 0x05;
				}

				usart_write_buffer_wait(&usart_instance, ack, sizeof(ack));
				break;
			}
		}
	}
}

void receive_frame() {
	
	uint8_t frame[2] = {0, 0};
		
	while(true) {
		if (usart_read_buffer_wait(&usart_instance, frame, sizeof(frame)) == STATUS_OK) {
			LED_Toggle(LED0);
			
			image_frame.height = frame[0];
			image_frame.width = frame[1];
			
			for(int i = 0; i<2;i++) {
				r_data[0][i] = frame[i];
			}
			
			usart_write_buffer_wait(&usart_instance, ack, sizeof(ack));
			break;
		}
	}
}

void receive_picture () {
	
	bool recvflag = true;
	bool sendflag = false;
	
	int line_num = 1;
	while (true) {
		
		if(recvflag) {
			if (usart_read_buffer_wait(&usart_instance, r_data[line_num], sizeof(r_data[line_num])) == STATUS_OK) {
				LED_Toggle(LED0);
				
				recvflag = false;
				sendflag = true;
				
				line_num++;
			}
		}
		
		if(sendflag) {
			usart_write_buffer_wait(&usart_instance, ack, sizeof(ack));
			
			sendflag = false;
			recvflag = true;
			
			if(line_num >= image_frame.height + 1) {
				line_num = 1;
				break;
			}
		}
	}
}

int main (void)
{
	irq_initialize_vectors();
	system_init();
	delay_init();
	SYS_Init();
	
	configure_usart();
	cpu_irq_enable();
	
	radioInit();
	
	while(true) {
		SYS_TaskHandler();
		receive_mode();
		
		if(artistMode[4] == 0x03) {
			receive_frame();
			receive_picture();	
			
			SYS_TimerStart(&sendM);
			SYS_TimerStart(&sendT);	
		}
		else if(artistMode[4] == 0x00 || artistMode[4] == 0x04 || artistMode[4] == 0x05) {
			SYS_TimerStart(&sendM);
		}
	}
}
