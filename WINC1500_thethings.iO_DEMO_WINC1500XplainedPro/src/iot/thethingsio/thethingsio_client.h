/*
 * \file
 *
 * \brief WINC1500 thethings.iO Example.
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
#ifndef ATMEL_THETHINGSIO_H_INCLUDE
#define ATMEL_THETHINGSIO_H_INCLUDE

#include <compiler.h>
#include "iot/http/http_client.h"
#include "iot/mqtt/mqtt.h"


#ifdef __cplusplus
extern "C" {
#endif

// format to send values to thethings.io
#define MAIN_THETHINGSIO_JSON_START					"{\"values\":[{"
#define MAIN_THETHINGSIO_JSON_KEY_VALUE_SS			"\"key\":\"%s\",\"value\":\"%s\""
#define MAIN_THETHINGSIO_JSON_KEY_VALUE_SI			"\"key\":\"%s\",\"value\":\"%d\""
	//#define MAIN_THETHINGSIO_JSON_KEY_VALUE_SMALL_SS	"\"%s\":\"%s\""						// minimum amount of characters for string-key and  string-value, returns error from thethings!!
	//#define MAIN_THETHINGSIO_JSON_KEY_VALUE_SMALL_SI	"\"%s\":%d"							// minimum amount of characters for string-key and integer-value, returns error from thethings!!
#define MAIN_THETHINGSIO_JSON_SEPARATOR				","
#define MAIN_THETHINGSIO_JSON_END					"}]}"


/* Max size of MQTT buffer. */
#define MAIN_MQTT_BUFFER_SIZE 128

#define MQTT_PORT 1883

typedef void (*thethingsio_http_cb)(struct http_client_module *module_inst, int type, union http_client_data *data);


/*
*	\brief Initialize thethingsio-demo.
*/
bool	thethingsio_example_http_init(thethingsio_http_cb cb);

/* check if thing has been configured already (thing token) */
uint8_t thethingsio_example_thing_token_available_nvm(void);
/* next step, load thing token from memory */
uint8_t thethingsio_example_load_thing_token_nvm(); 
/* if not, save new thing token to memory and use it */
uint8_t thethingsio_example_write_thing_token_nvm(char * thing_token);

//uint8_t thethingsio_example_subscribe_things_token_nvm(char * thing_token);

/*
*	\brief Send message to thethingsio server for activation of the thing.
*
*	\param activation_code	device activation code.
*
*	\return	true send message to server successfully.
*/
bool	thethingsio_example_activate_thing(char * activation_code);

/*
*	\brief Send message to thethings.io server.
*
*
*	\return	true send message to server. 
*/
bool	thethingsio_example_read_and_write(char * write_data);

/*
*	\brief Subscribe to the MQTT queue of the server.
*
*
*	\return	true the subscription was done.
*/



/* @typedef \
	tpfAppWifiCb

@brief
				Wi-Fi's main callback function handler, for handling the M2M_WIFI events received on the Wi-Fi interface.
			       Such notifications are received in response to Wi-Fi/P2P operations such as @ref m2m_wifi_request_scan,
				@ref m2m_wifi_connect.
				Wi-Fi/P2P operations are implemented in an asynchronous mode, and all incoming information/status
				are to be handled through this callback function when the corresponding notification is received.
				Applications are expected to assign this wi-fi callback function by calling @ref m2m_wifi_init
@param [in]	u8MsgType
				Type of notification. Possible types are:
				/ref M2M_WIFI_RESP_CON_STATE_CHANGED
				/ref M2M_WIFI_RESP_CONN_INFO
				/ref M2M_WIFI_REQ_DHCP_CONF
				/ref M2M_WIFI_REQ_WPS
				/ref M2M_WIFI_RESP_IP_CONFLICT
				/ref M2M_WIFI_RESP_SCAN_DONE
				/ref M2M_WIFI_RESP_SCAN_RESULT
				/ref M2M_WIFI_RESP_CURRENT_RSSI
				/ref M2M_WIFI_RESP_CLIENT_INFO
				/ref M2M_WIFI_RESP_PROVISION_INFO
				/ref M2M_WIFI_RESP_DEFAULT_CONNECT

			In case Bypass mode is defined :
				@ref M2M_WIFI_RESP_ETHERNET_RX_PACKET

			In case Monitoring mode is used:
				@ref M2M_WIFI_RESP_WIFI_RX_PACKET

@param [in]	pvMsg
				A pointer to a buffer containing the notification parameters (if any). It should be
				Casted to the correct data type corresponding to the notification type.

@see
	tstrM2mWifiStateChanged
	tstrM2MWPSInfo
	tstrM2mScanDone
	tstrM2mWifiscanResult
*/

typedef void (*thethingsio_subscribe_cb) (char* message);

/* @typedef \
	tpfAppWifiCb

@brief
				Wi-Fi's main callback function handler, for handling the M2M_WIFI events received on the Wi-Fi interface.
			       Such notifications are received in response to Wi-Fi/P2P operations such as @ref m2m_wifi_request_scan,
				@ref m2m_wifi_connect.
				Wi-Fi/P2P operations are implemented in an asynchronous mode, and all incoming information/status
				are to be handled through this callback function when the corresponding notification is received.
				Applications are expected to assign this wi-fi callback function by calling @ref m2m_wifi_init
@param [in]	u8MsgType
				Type of notification. Possible types are:
				/ref M2M_WIFI_RESP_CON_STATE_CHANGED
				/ref M2M_WIFI_RESP_CONN_INFO
				/ref M2M_WIFI_REQ_DHCP_CONF
				/ref M2M_WIFI_REQ_WPS
				/ref M2M_WIFI_RESP_IP_CONFLICT
				/ref M2M_WIFI_RESP_SCAN_DONE
				/ref M2M_WIFI_RESP_SCAN_RESULT
				/ref M2M_WIFI_RESP_CURRENT_RSSI
				/ref M2M_WIFI_RESP_CLIENT_INFO
				/ref M2M_WIFI_RESP_PROVISION_INFO
				/ref M2M_WIFI_RESP_DEFAULT_CONNECT

			In case Bypass mode is defined :
				@ref M2M_WIFI_RESP_ETHERNET_RX_PACKET

			In case Monitoring mode is used:
				@ref M2M_WIFI_RESP_WIFI_RX_PACKET

@param [in]	pvMsg
				A pointer to a buffer containing the notification parameters (if any). It should be
				Casted to the correct data type corresponding to the notification type.

@see
	tstrM2mWifiStateChanged
	tstrM2MWPSInfo
	tstrM2mScanDone
	tstrM2mWifiscanResult
*/

typedef void (*tpfTheThingsIOReadcallback) (char* message, char* data);


/* @typedef \
	tpfAppWifiCb

@brief
				Wi-Fi's main callback function handler, for handling the M2M_WIFI events received on the Wi-Fi interface.
			       Such notifications are received in response to Wi-Fi/P2P operations such as @ref m2m_wifi_request_scan,
				@ref m2m_wifi_connect.
				Wi-Fi/P2P operations are implemented in an asynchronous mode, and all incoming information/status
				are to be handled through this callback function when the corresponding notification is received.
				Applications are expected to assign this wi-fi callback function by calling @ref m2m_wifi_init
@param [in]	u8MsgType
				Type of notification. Possible types are:
				/ref M2M_WIFI_RESP_CON_STATE_CHANGED
				/ref M2M_WIFI_RESP_CONN_INFO
				/ref M2M_WIFI_REQ_DHCP_CONF
				/ref M2M_WIFI_REQ_WPS
				/ref M2M_WIFI_RESP_IP_CONFLICT
				/ref M2M_WIFI_RESP_SCAN_DONE
				/ref M2M_WIFI_RESP_SCAN_RESULT
				/ref M2M_WIFI_RESP_CURRENT_RSSI
				/ref M2M_WIFI_RESP_CLIENT_INFO
				/ref M2M_WIFI_RESP_PROVISION_INFO
				/ref M2M_WIFI_RESP_DEFAULT_CONNECT

			In case Bypass mode is defined :
				@ref M2M_WIFI_RESP_ETHERNET_RX_PACKET

			In case Monitoring mode is used:
				@ref M2M_WIFI_RESP_WIFI_RX_PACKET

@param [in]	pvMsg
				A pointer to a buffer containing the notification parameters (if any). It should be
				Casted to the correct data type corresponding to the notification type.

@see
	tstrM2mWifiStateChanged
	tstrM2MWPSInfo
	tstrM2mScanDone
	tstrM2mWifiscanResult
*/
typedef void (*tpfTheThingsIOWritecallback) (uint8_t uiMessage);



/* @typedef \
	tpfAppWifiCb

@brief
				Wi-Fi's main callback function handler, for handling the M2M_WIFI events received on the Wi-Fi interface.
			       Such notifications are received in response to Wi-Fi/P2P operations such as @ref m2m_wifi_request_scan,
				@ref m2m_wifi_connect.
				Wi-Fi/P2P operations are implemented in an asynchronous mode, and all incoming information/status
				are to be handled through this callback function when the corresponding notification is received.
				Applications are expected to assign this wi-fi callback function by calling @ref m2m_wifi_init
@param [in]	u8MsgType
				Type of notification. Possible types are:
				/ref M2M_WIFI_RESP_CON_STATE_CHANGED
				/ref M2M_WIFI_RESP_CONN_INFO
				/ref M2M_WIFI_REQ_DHCP_CONF
				/ref M2M_WIFI_REQ_WPS
				/ref M2M_WIFI_RESP_IP_CONFLICT
				/ref M2M_WIFI_RESP_SCAN_DONE
				/ref M2M_WIFI_RESP_SCAN_RESULT
				/ref M2M_WIFI_RESP_CURRENT_RSSI
				/ref M2M_WIFI_RESP_CLIENT_INFO
				/ref M2M_WIFI_RESP_PROVISION_INFO
				/ref M2M_WIFI_RESP_DEFAULT_CONNECT

			In case Bypass mode is defined :
				@ref M2M_WIFI_RESP_ETHERNET_RX_PACKET

			In case Monitoring mode is used:
				@ref M2M_WIFI_RESP_WIFI_RX_PACKET

@param [in]	pvMsg
				A pointer to a buffer containing the notification parameters (if any). It should be
				Casted to the correct data type corresponding to the notification type.

@see
	tstrM2mWifiStateChanged
	tstrM2MWPSInfo
	tstrM2mScanDone
	tstrM2mWifiscanResult
*/
//bool thethingsio_subscribe_config();

bool thethingsio_subscribe_config(thethingsio_subscribe_cb cb);


/* @typedef \
	tpfAppWifiCb

@brief
				Wi-Fi's main callback function handler, for handling the M2M_WIFI events received on the Wi-Fi interface.
			       Such notifications are received in response to Wi-Fi/P2P operations such as @ref m2m_wifi_request_scan,
				@ref m2m_wifi_connect.
				Wi-Fi/P2P operations are implemented in an asynchronous mode, and all incoming information/status
				are to be handled through this callback function when the corresponding notification is received.
				Applications are expected to assign this wi-fi callback function by calling @ref m2m_wifi_init
@param [in]	u8MsgType
				Type of notification. Possible types are:
				/ref M2M_WIFI_RESP_CON_STATE_CHANGED
				/ref M2M_WIFI_RESP_CONN_INFO
				/ref M2M_WIFI_REQ_DHCP_CONF
				/ref M2M_WIFI_REQ_WPS
				/ref M2M_WIFI_RESP_IP_CONFLICT
				/ref M2M_WIFI_RESP_SCAN_DONE
				/ref M2M_WIFI_RESP_SCAN_RESULT
				/ref M2M_WIFI_RESP_CURRENT_RSSI
				/ref M2M_WIFI_RESP_CLIENT_INFO
				/ref M2M_WIFI_RESP_PROVISION_INFO
				/ref M2M_WIFI_RESP_DEFAULT_CONNECT

			In case Bypass mode is defined :
				@ref M2M_WIFI_RESP_ETHERNET_RX_PACKET

			In case Monitoring mode is used:
				@ref M2M_WIFI_RESP_WIFI_RX_PACKET

@param [in]	pvMsg
				A pointer to a buffer containing the notification parameters (if any). It should be
				Casted to the correct data type corresponding to the notification type.

@see
	tstrM2mWifiStateChanged
	tstrM2MWPSInfo
	tstrM2mScanDone
	tstrM2mWifiscanResult
*/

bool thethingsio_connect_subscribe();



/* @typedef \
	tpfAppWifiCb

@brief
				Wi-Fi's main callback function handler, for handling the M2M_WIFI events received on the Wi-Fi interface.
			       Such notifications are received in response to Wi-Fi/P2P operations such as @ref m2m_wifi_request_scan,
				@ref m2m_wifi_connect.
				Wi-Fi/P2P operations are implemented in an asynchronous mode, and all incoming information/status
				are to be handled through this callback function when the corresponding notification is received.
				Applications are expected to assign this wi-fi callback function by calling @ref m2m_wifi_init
@param [in]	u8MsgType
				Type of notification. Possible types are:
				/ref M2M_WIFI_RESP_CON_STATE_CHANGED
				/ref M2M_WIFI_RESP_CONN_INFO
				/ref M2M_WIFI_REQ_DHCP_CONF
				/ref M2M_WIFI_REQ_WPS
				/ref M2M_WIFI_RESP_IP_CONFLICT
				/ref M2M_WIFI_RESP_SCAN_DONE
				/ref M2M_WIFI_RESP_SCAN_RESULT
				/ref M2M_WIFI_RESP_CURRENT_RSSI
				/ref M2M_WIFI_RESP_CLIENT_INFO
				/ref M2M_WIFI_RESP_PROVISION_INFO
				/ref M2M_WIFI_RESP_DEFAULT_CONNECT

			In case Bypass mode is defined :
				@ref M2M_WIFI_RESP_ETHERNET_RX_PACKET

			In case Monitoring mode is used:
				@ref M2M_WIFI_RESP_WIFI_RX_PACKET

@param [in]	pvMsg
				A pointer to a buffer containing the notification parameters (if any). It should be
				Casted to the correct data type corresponding to the notification type.

@see
	tstrM2mWifiStateChanged
	tstrM2MWPSInfo
	tstrM2mScanDone
	tstrM2mWifiscanResult
*/
bool thethingsio_refresh_subscribe();

bool thethingsio_disconnect_subscribe();

bool thethingsio_example_subscribe(char* topic);


struct client_thethingsio_module {
	
	/** A flag for the socket is sending. */
	uint8_t subscribe_status :  1;
	/** A flag that whether using the persistent connection or not. */
	uint8_t http_Status   : 1;

	//thethingsio_http_cb http_cb;
	
	thethingsio_subscribe_cb subscribe_cb;
};

#ifdef __cplusplus
}
#endif

#endif /* ATMEL_THETHINGSIO_H_ */