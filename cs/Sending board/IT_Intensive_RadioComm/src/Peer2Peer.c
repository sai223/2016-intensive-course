/**
* \file
*
* \brief Empty user application template
*
*/

/**
* \mainpage User Application template doxygen documentation
*
* \par Empty user application template
*
* This is a bare minimum user application template.
*
* For documentation of the board, go \ref group_common_boards "here" for a link
* to the board-specific documentation.
*
* \par Content
*
* -# Include the ASF header files (through asf.h)
* -# Minimal main function that starts with a call to system_init()
* -# Basic usage of on-board LED and button
* -# "Insert application code here" comment
*
*/

/*
* Include header files for all drivers that have been imported from
* Atmel Software Framework (ASF).
*/
/*
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/
#include <asf.h>
#include <string.h>

#include "asf.h"
#include "phy.h"
#include "nwk.h"
#include "config.h"
#include "sys.h"
#include "systimer.h"

static SYS_Timer_t sendT;
static SYS_Timer_t sendStart;

struct usart_module usart_instance;
//static struct usart_module host_uart_module;

static NWK_DataReq_t appDataReq;
static bool sendBusy= false; // flag 역할 죽을수도 있어서

uint8_t height;
uint8_t width;

uint8_t mode[4];
uint8_t frame[2];

uint8_t r_data[50][51;

uint8_t ack[4] = "okay";


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

static bool receivePKT(NWK_DataInd_t *ind) {
	
	printf("data:%s\n", ind->data);

	LED_Toggle(LED0);
	return true;
}

static void sendDonePKT(NWK_DataReq_t *req) {
	sendBusy = false;
	printf("sendDonePKT\n");
}

static void sendFrame(void) {
	
	if(sendBusy)
	return;
	
	appDataReq.dstAddr = 9;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = r_data[line_count];
	appDataReq.size = 51;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	//LED_Toggle(LED0);
	sendBusy = true;
	line_count++;
	if(line_count == 50) {
		SYS_TimerStop(&sendT);
		line_count = 0;
	}
	
	printf("sendPKT");
}

static void sendPKT(void) {
	
	if(sendBusy)
	return;
	
	appDataReq.dstAddr = 9;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = r_data[line_count];
	appDataReq.size = 51;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	//LED_Toggle(LED0);
	sendBusy = true;
	line_count++;
	if(line_count == 50) {
		SYS_TimerStop(&sendT);
		line_count = 0;
	}
	
	printf("sendPKT");
}

static void radioInit(void) {
	NWK_SetAddr(10);  //주소 설정
	NWK_SetPanId(APP_PANID);  //PANID : Personal Area Network ID
	PHY_SetChannel(APP_CHANNEL);
	PHY_SetRxState(true);
	NWK_OpenEndpoint(APP_ENDPOINT, receivePKT);
	
	sendT.interval = 200; //interval 200ms 한번, periodic 200ms 간격으로 계속 쏘는거
	sendT.mode =SYS_TIMER_PERIODIC_MODE;
	sendT.handler = sendPKT;
	SYS_TimerStart(&sendT); // timer 시작
	//SYS_TimerStop(&sendT);  // timer 멈추기
}

void receive_picture () {
	
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

	bool sendflag = false;
	bool recvflag = true;
	
	while(true) {
		//printf("flag");
		if (usart_read_buffer_wait(&usart_instance, mode, sizeof(mode)) == STATUS_OK) {
			LED_Toggle(LED0);
			usart_write_buffer_wait(&usart_instance, ack, sizeof(ack));
			
			if(!strcmp(mode, "draw")) {
				
				//Start receive start packet from computer
				while(true) {
					//printf("draw okay");
					if (usart_read_buffer_wait(&usart_instance, frame, sizeof(frame)) == STATUS_OK) {
						LED_Toggle(LED0);
						height = frame[0];
						width = frame[1];
						usart_write_buffer_wait(&usart_instance, ack, sizeof(ack));
						break;
					}
				}
				//End receive start packet from computer
				
				//Start receiving Bit Packets from computer
				int line_num = 0;
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
						for(int i = 0;i<1;i++) {
							usart_write_buffer_wait(&usart_instance, ack, sizeof(ack));
						}
						//printf("hello python");
						sendflag = false;
						recvflag = true;
					}
					
					if(line_num >= height)
					break;
				}
				
				sendPKT();
			}
			
			else if(!strcmp(mode, "maze")) {
				
			}
			

		}
	}
	
	//Receive start packet from computer
	
	/*
	while(true) {
		delay_ms(1500);
		for(int i = 0; i < height; i++) {
			printf("\n");
			for(int j=0; j<51;j++) {
				printf("%d ", r_data[i][j]);
			}
		}
		printf("\n");
	}
	*/
	while (1) {
		SYS_TaskHandler();
	}

}
