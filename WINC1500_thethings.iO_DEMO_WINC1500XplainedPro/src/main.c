/*
 * \file
 *
 * \brief WINC1500 TheThings.iO Example.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/** \mainpage
 * \section intro Introduction
 * This example demonstrates the use of the WINC1500 with the SAMD21 Xplained Pro
 * board to retrieve the chip information of the Wi-Fi module.<br>
 * It uses the following hardware:
 * - the SAMD21 Xplained Pro.
 * - the WINC1500 on EXT1.
 * The read/write and subscribe functionality added in the example to connect
 * to thethings.io platform.
 *
 * \section files Main Files
 * - main.c : Initialize the WINC1500 and retrieve information.
 *
 * \section usage Usage
 * -# Build the program and download it into the board.
 * -# On the computer, open and configure a terminal application as the follows.
 * \code
 *    Baud Rate : 115200
 *    Data : 8bit
 *    Parity bit : none
 *    Stop bit : 1bit
 *    Flow control : none
 * \endcode
 * -# Start the application.
 * -# In the terminal window, the following text should appear:
 * \code
 *    -- WINC1500 chip information example --
 *    -- SAMD21_XPLAINED_PRO --
 *    -- Compiled: xxx xx xxxx xx:xx:xx --
 *    Chip ID :             xxxxxx
 *    RF Revision ID :      x
 *    Done.
 * \endcode
 *
 * \section compinfo Compilation Information
 * This software was written for the GNU GCC compiler using Atmel Studio 6.2
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com">Atmel</A>.\n
 */

#include "asf.h"
#include "debug_conf.h"
#include "driver/include/m2m_wifi.h"
#include "driver/source/nmasic.h"
#include "socket/include/socket.h"
#include "main.h"
#include "conf_nvm.h"
#include "tick_counter.h"
#include "thethingsio/thethingsio_client.h"

/** state enum value for sequence*/
typedef enum {
	MAIN_NEED_TO_CHECK_AP_INFORMATION,		/* state before check the restored AP information in the chip */
	MAIN_CHECKING_AP_INFORMATION,			/* state to be checking the restored AP information in the chip */
	MAIN_NEED_HTTP_PROVISIONING,			/* state to need to do the HTTP Provisioning */ 
	MAIN_DOING_AP_HTTP_PROVISIONING,		/* state to The HTTP Provision mode */
	MAIN_AP_CONNECTION,						/* state to be connected AP after HTTP Provisioning */
} wifi_connect_state;

/** AP Connection state. */
static wifi_connect_state ap_wifi_connection_state = MAIN_NEED_TO_CHECK_AP_INFORMATION;

/** UART module for debug. */
static struct usart_module cdc_uart_module;





/**
* \brief Copy MAC address to string. 
*/ 
static void set_dev_name_to_mac(uint8 *name, uint8 *mac_addr)
{
	/* Name must be in the format WINC1500_00:00 */
	uint16 len;

	len = m2m_strlen(name);
	if (len >= 5) {
		name[len - 1] = HEX2ASCII((mac_addr[5] >> 0) & 0x0f);
		name[len - 2] = HEX2ASCII((mac_addr[5] >> 4) & 0x0f);
		name[len - 4] = HEX2ASCII((mac_addr[4] >> 0) & 0x0f);
		name[len - 5] = HEX2ASCII((mac_addr[4] >> 4) & 0x0f);
	}
}

/**
 * \brief Configure UART console.
 */
static void configure_console(void)
{
	struct usart_config usart_conf;

	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = EDBG_CDC_SERCOM_MUX_SETTING;
	usart_conf.pinmux_pad0 = EDBG_CDC_SERCOM_PINMUX_PAD0;
	usart_conf.pinmux_pad1 = EDBG_CDC_SERCOM_PINMUX_PAD1;
	usart_conf.pinmux_pad2 = EDBG_CDC_SERCOM_PINMUX_PAD2;
	usart_conf.pinmux_pad3 = EDBG_CDC_SERCOM_PINMUX_PAD3;
	usart_conf.baudrate    = 115200;

	stdio_serial_init(&cdc_uart_module, EDBG_CDC_MODULE, &usart_conf);
	usart_enable(&cdc_uart_module);
}



/**
 * \brief Configure the gpio pins to activate on/off
 */
static void config_led(void)
{
	struct port_config pin_conf;
	port_get_config_defaults(&pin_conf);
	pin_conf.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(EXT2_PIN_GPIO_0, &pin_conf);
	port_pin_set_output_level(EXT2_PIN_GPIO_0, false);			
	port_pin_set_config(EXT2_PIN_GPIO_1, &pin_conf);
	port_pin_set_output_level(EXT2_PIN_GPIO_1, false);
}

/**
 * \brief Callback to get the Wi-Fi status update.
 *
 * \param[in] u8MsgType type of Wi-Fi notification. Possible types are:
 *  - [M2M_WIFI_RESP_CON_STATE_CHANGED](@ref M2M_WIFI_RESP_CON_STATE_CHANGED)
 *  - [M2M_WIFI_REQ_DHCP_CONF](@ref M2M_WIFI_REQ_DHCP_CONF)
 * \param[in] pvMsg A pointer to a buffer containing the notification parameters
 * (if any). It should be casted to the correct data type corresponding to the
 * notification type.
 */
static void wifi_cb(uint8_t u8MsgType, void* pvMsg) 
{
	// DEBUG(DEBUG_CONF_WIFI "call wifi_callback function" DEBUG_EOL);
	switch(u8MsgType) 
	{
		case M2M_WIFI_RESP_CURRENT_RSSI:
		{
			int8_t *rssi = (int8_t *)pvMsg;
			printf("RSSI for the current connected AP (%d)\r\n",
			(int8_t)(*rssi));
		}		
		case M2M_WIFI_RESP_CON_STATE_CHANGED:
		{
			tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
			
			DEBUG(DEBUG_CONF_WIFI "case M2M_WIFI_RESP_CON_STATE_CHANGED" DEBUG_EOL );
			if( pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED)
			{
				
				 m2m_wifi_request_dhcp_client();
			}
			else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED)
			{
				
				DEBUG(DEBUG_CONF_SOCKET"Wi-Fi disconnected"DEBUG_EOL);
			}
			break;
		}
		case M2M_WIFI_REQ_DHCP_CONF:
		{
			uint8_t *pu8IPAddress = (uint8_t*)pvMsg;
			printf("Wi-Fi connected\r\n");
			printf("Wi-Fi IP is %u.%u.%u.%u\r\n", pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
	
			DEBUG(DEBUG_CONF_WIFI "case M2M_WIFI_REQ_DHCP_CONF" DEBUG_EOL );
			DEBUG(DEBUG_CONF_WIFI "GET IP :is %u.%u.%u.%u" DEBUG_EOL, pu8IPAddress[0],pu8IPAddress[1],pu8IPAddress[2],pu8IPAddress[3]);
			ap_wifi_connection_state = MAIN_AP_CONNECTION;
			
			
			// configure thing token 
			if( thethingsio_example_thing_token_available_nvm() )
			{
				thethingsio_set_thingtoken_state(0x02);
				thethingsio_example_load_thing_token_nvm();
				
				// MQTT notifications: call thethingsio_connect_subscribe. First we need the thingtoken loaded or recieved.
				if (THETHINGSIO_MQTT_SUBSCRIPTION_ACTIVATE)
				{
					thethingsio_connect_subscribe();
				}
				
				printf("thing token loaded from nvm successfully \n\r");
			}
			else
			{
				// request thing token sending auth code
				thethingsio_set_thingtoken_state(0x01);
				thethingsio_activate_thing();
				printf("thing token request sent with activation code\n\r");
			}
			
			break;
		}
		case M2M_WIFI_RESP_DEFAULT_CONNECT:
		{
			tstrM2MDefaultConnResp *pstrDefConnInfo = (tstrM2MDefaultConnResp *)pvMsg;
			DEBUG(DEBUG_CONF_WIFI "M2M_WIFI_RESP_DEFAULT_CONNECT."DEBUG_EOL);
			if (pstrDefConnInfo->s8ErrorCode != M2M_SUCCESS) {
				ap_wifi_connection_state = MAIN_NEED_HTTP_PROVISIONING;
				DEBUG(DEBUG_CONF_WIFI "Set MAIN_NEED_HTTP_PROVISIONING mode "DEBUG_EOL);
			}
			else
			{
				ap_wifi_connection_state = MAIN_AP_CONNECTION;
				DEBUG(DEBUG_CONF_WIFI "Set MAIN_AP_CONNECTION mode "DEBUG_EOL);
				tick_counter_reset_timer(100);
			}
			break;		
		}
		case M2M_WIFI_RESP_PROVISION_INFO:
		{
			tstrM2MProvisionInfo *pstrProvInfo = (tstrM2MProvisionInfo *)pvMsg;
			DEBUG(DEBUG_CONF_WIFI "M2M_WIFI_RESP_PROVISION_INFO."DEBUG_EOL);

			if (pstrProvInfo->u8Status == M2M_SUCCESS) 
			{			
				m2m_wifi_connect((char *)pstrProvInfo->au8SSID, strlen((char *)pstrProvInfo->au8SSID), pstrProvInfo->u8SecType,
				pstrProvInfo->au8Password, M2M_WIFI_CH_ALL);
				DEBUG(DEBUG_CONF_WIFI "HTTP PROVISION Success."DEBUG_EOL);
			}
			else
			{
				DEBUG(DEBUG_CONF_WIFI "HTTP PROVISION  Failed."DEBUG_EOL);
				ap_wifi_connection_state = MAIN_NEED_HTTP_PROVISIONING;
			}
			break;
		}		
		default:
		{
			DEBUG(DEBUG_CONF_WIFI "call wifi_callback function msg type : %d" DEBUG_EOL, u8MsgType);
			break;
		}
	}
}




/**
 * \brief Callback of the MQTT client. ThethingsIO platform send broadcast the messages which the thing recieve.
 *
 * \param[in]  message     The jaon message recieve in the MQTT channel from thethings platform.
 *
 */
static void thethingsio_subscribe_callback(char* message)
{
	
   // this callback change the GPIO pin EXT2 en la extension board
   // check to callback with 
   // curl -i -H "Accept: application/json"  -H "Content-Type: application/json"  -d '{"values":[{"key": "green","value": "1"}]}' -X POST "http://api.thethings.io/v2/things/{THINGTOKEN}" -k 	
   printf("%s", message);
   char strongreen[35];
   char stroffgreen[35];

   int ret;
  // port_pin_toggle_output_level(EXT2_PIN_GPIO_0);
   strcpy(stroffgreen, "[{\"key\":\"green\",\"value\":\"0\"}]\034");
   strcpy(strongreen, "[{\"key\":\"green\",\"value\":\"1\"}]\034");     
   ret = strncmp(stroffgreen, message, strnlen(message, 150));
   if (ret == 0)
   {
	    printf("green off \n\r");
		port_pin_set_output_level(EXT2_PIN_GPIO_0, false);
		return;
		
   }
   ret = strncmp(strongreen, message, strnlen(message, 150));
   if (ret == 0)
   {
	    printf("green on \n\r");
		port_pin_set_output_level(EXT2_PIN_GPIO_0, true);
		return;	
	}
}

/**
 * \brief Callback of the HTTP client.
 *
 * \param[in]  module_inst     Module instance of HTTP client module.
 * \param[in]  type            Type of event.
 * \param[in]  data            Data structure of the event. \refer http_client_data
 */
static void main_http_client_callback(struct http_client_module *module_inst, int type, union http_client_data *data)
{
	switch (type) {
	case HTTP_CLIENT_CALLBACK_SOCK_CONNECTED:
		DEBUG(DEBUG_CONF_THETHINGSIO"TheThingsIO Server Connected"DEBUG_EOL);
		break;

	case HTTP_CLIENT_CALLBACK_REQUESTED:
		DEBUG(DEBUG_CONF_THETHINGSIO"Request complete"DEBUG_EOL);
		break;

	case HTTP_CLIENT_CALLBACK_RECV_RESPONSE:
		tick_counter_reset_timer(TICK_COUNTER_INTERVAL);
		thethingsio_parsing_http_response_data(data->recv_response.response_code, data->recv_response.content, data->recv_response.content_length);
		break;

	case HTTP_CLIENT_CALLBACK_DISCONNECTED:
		{
			tick_counter_reset_timer(TICK_COUNTER_INTERVAL);
			DEBUG(DEBUG_CONF_THETHINGSIO"Disconnected reason:%d"DEBUG_EOL, data->disconnected.reason);
			break;
		}
	}
}

/**
 * \brief Main application function.
 * Application entry point.
 * \return program return value.
 */
int main(void)
{
	tstrWifiInitParam param;
	int16_t ret;

	/* Initialize the board. */
	system_init();

	/* setting sleep mode. */
	system_set_sleepmode(SYSTEM_SLEEPMODE_IDLE_0);
	
	/* Initialize the LED */
	config_led();
	
	/* Initialize the UART console. */
	configure_console();
	DEBUG(DEBUG_EOL DEBUG_EOL DEBUG_STRING_HEADER);	
	
	/* Initialize temperature sensor */
	at30tse_init();
	
	/* Configure Non-Volatile Memory */
	configure_nvm();
	
	if (THETHINGSIO_MQTT_SUBSCRIPTION_ACTIVATE)
	{
		/* Configure the MQTT subscribe broker */
		thethingsio_subscribe_config(thethingsio_subscribe_callback);
	}
	

	
	/* Initialize TheThings.iO */
	thethingsio_example_http_init(main_http_client_callback);
	
	/* Initialize the BSP. */
	nm_bsp_init();
	
	/* Enable SysTick interrupt for non busy wait delay. */
	if (SysTick_Config(system_cpu_clock_get_hz() / 1000))
	{
		DEBUG("SysTick configuration error" DEBUG_EOL);
		while(1);
	}
	
	/* Initialize Wi-Fi parameters structure. */
	memset((uint8_t *)&param, 0, sizeof(tstrWifiInitParam));
	
	/* Initialize WINC1500 Wi-Fi driver with data and status callbacks. */
	param.pfAppWifiCb = wifi_cb;
	ret = m2m_wifi_init(&param);
	if (M2M_SUCCESS != ret) {
		DEBUG(DEBUG_CONF_WIFI "m2m_wifi_init call error!(%d)" DEBUG_EOL, ret);
		while (1) {
		}
	}
		
	m2m_wifi_set_sleep_mode(MAIN_PS_SLEEP_MODE, 1);
		
	/* Initialize socket. */
	socketInit();
	
	// We have 2 Callbacks one for the MQTT subsystem and another for the HTTP request.
	//registerSocketCallback(socket_cb, resolve_cb);
	if (THETHINGSIO_MQTT_SUBSCRIPTION_ACTIVATE)
	{
		registerSocketCallbackMQTT(mqtt_socket_event_handler, mqtt_socket_resolve_handler);
	}
	
	registerSocketCallback(http_client_socket_event_handler, http_client_socket_resolve_handler);
	
	// WIFI Connection
	 // m2m_wifi_default_connect();	
	m2m_wifi_connect((char *)MAIN_WLAN_DEVICE_NAME, strlen((char *)MAIN_WLAN_DEVICE_NAME), M2M_WIFI_SEC_WPA_PSK, (char *)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);
	
	printf("Connecting to %s.\r\n", (char *)MAIN_WLAN_DEVICE_NAME);
	
	ap_wifi_connection_state = MAIN_CHECKING_AP_INFORMATION;
	
	for(;;system_sleep()) 
	{		
		/* Handle pending events from network controller. */
		ret = m2m_wifi_handle_events(NULL);		
		if( ret != M2M_SUCCESS ) 
		{	
			DEBUG(DEBUG_CONF_WIFI "wifi_handle_event error code :%d"DEBUG_EOL, ret);
		}
		
		/* Check Wi-Fi AP connection state. */
		if( ap_wifi_connection_state != MAIN_AP_CONNECTION) 
		{
			if( ap_wifi_connection_state == MAIN_NEED_TO_CHECK_AP_INFORMATION )
			{
				m2m_wifi_default_connect();
				ap_wifi_connection_state = MAIN_CHECKING_AP_INFORMATION;
				DEBUG(DEBUG_CONF_WIFI "m2m_wifi_default_connect start"DEBUG_EOL);
			}
			else if (ap_wifi_connection_state == MAIN_NEED_HTTP_PROVISIONING)
			{
				uint8_t mac_addr[6];
				uint8_t u8IsMacAddrValid;
				
				m2m_wifi_get_otp_mac_address(mac_addr, &u8IsMacAddrValid);
				if (!u8IsMacAddrValid) {
					m2m_wifi_set_mac_address(gau8MacAddr);
				}
				
				m2m_wifi_get_mac_address(gau8MacAddr);
				set_dev_name_to_mac((uint8_t *)gacDeviceName, gau8MacAddr);
				//set_dev_name_to_mac((uint8_t *)gstrM2MAPConfig.au8SSID, gau8MacAddr);
				
				m2m_wifi_set_device_name((uint8_t *)gacDeviceName, (uint8_t)m2m_strlen((uint8_t *)gacDeviceName));
				
				// m2m_wifi_start_provision_mode((tstrM2MAPConfig *)&gstrM2MAPConfig, (char *)gacHttpProvDomainName, 1);
				ap_wifi_connection_state = MAIN_DOING_AP_HTTP_PROVISIONING;
				DEBUG(DEBUG_CONF_WIFI "m2m_wifi_start_provision_mode start"DEBUG_EOL);
			}
			continue;
		} 
				
		/* do things if thing token has been configured correctly */
		if( tick_counter_check_timer() && thethingsio_get_thingtoken_state() == 0x02 )
		{	
			char send_buf[100] = {0,};
			int dTemp = 0;
				
			dTemp = (int) at30tse_read_temperature();		
			sprintf(send_buf,MAIN_THETHINGSIO_JSON_START MAIN_THETHINGSIO_JSON_KEY_VALUE_SI MAIN_THETHINGSIO_JSON_END, "temperature", (int)dTemp);
			
			
			// send temperature value
			thethingsio_example_read_and_write(send_buf);	
		}
	}

	return 0;
}




