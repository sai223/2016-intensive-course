/*
 * conf_radio.h
 *
 * Created: 4/18/2017 9:41:28 PM
 *  Author: credt
 */ 


#ifndef CONF_RADIO_H_
#define CONF_RADIO_H_


/*- Definitions ------------------------------------------------------------*/

/* Address must be set to 0 for the first device, and to 1 for the second one.
 **/
#define APP_ADDR                  1
#define APP_PANID                 0x4567
#define APP_ENDPOINT              1
#define APP_SECURITY_KEY          "TestSecurityKey0"
#define APP_FLUSH_TIMER_INTERVAL  20

#ifdef PHY_AT86RF212
  #define APP_CHANNEL             0x1A
  #define APP_BAND                0x00
  #define APP_MODULATION          0x24
#else
  #define APP_CHANNEL             0x0f
#endif

#define SYS_SECURITY_MODE                   0
#define APP_RX_BUF_SIZE                     20
#define NWK_BUFFERS_AMOUNT                  3
#define NWK_DUPLICATE_REJECTION_TABLE_SIZE  10
#define NWK_DUPLICATE_REJECTION_TTL         3000 /* ms */
#define NWK_ROUTE_TABLE_SIZE                100
#define NWK_ROUTE_DEFAULT_SCORE             3
#define NWK_ACK_WAIT_TIME                   1000 /* ms */

#define NWK_ENABLE_ROUTING
#define NWK_ENABLE_SECURITY




#endif /* CONF_RADIO_H_ */