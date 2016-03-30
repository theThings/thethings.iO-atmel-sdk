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


// #define DEBUG_THING_TOKEN							"XJgU-PTj5k9VRvl_5I3O8mIkFNSYBpk7JMWRpQrNwQE"
#define THETHINGSIO_MQTT_SUBSCRIPTION_ACTIVATE       1

// this activation code once requests a thing token. This thing token will then be stored in non volatile memory
// IMPORTANT !!!!!! you should modify the following line with your own activation code.
//#define MAIN_THETHINGSIO_ACTIVATION_CODE			"uPyFIj1aaVMPW5PmrtXoBZ7Oiz9XJYJ_9A"  // 
#define MAIN_THETHINGSIO_ACTIVATION_CODE			"AYko3JsNbtEQoHTePzuaRJp0T0TcjGT5ZQ"  // 


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
*    @typedef \
*         thethingsio_subscribe_cb callback
*
*	@brief Subscribe to the MQTT queue of the server.
*
*
*   @param [in]	message  the json message from thethings.io platform.
*
*
*	@return	true the subscription was done.
*/
typedef void (*thethingsio_subscribe_cb) (char* message);


/*
*    @typedef \
*         tpfTheThingsIOReadcallback callback
*
*	@brief callback from the RESST io api
*
*
*   @param [in]	message  the json message from thethings.io platform.
*
*
*	@return	true the subscription was done.
*/
typedef void (*tpfTheThingsIOReadcallback) (char* message, char* data);


typedef void (*tpfTheThingsIOWritecallback) (uint8_t uiMessage);



/* @typedef \
	tpfAppWifiCb

@brief		configure the subscription callback for the MQTT broker.



activate the MQTT system.

*/

bool thethingsio_subscribe_config(thethingsio_subscribe_cb cb);


/* @typedef \
	tpfAppWifiCb

@brief
			

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

/* @typedef \
	tpfAppWifiCb

@brief
				

@param [in]	pvMsg
				A pointer to a buffer containing the notification parameters (if any). It should be
				Casted to the correct data type corresponding to the notification type.

@see
	tstrM2mWifiStateChanged
	tstrM2MWPSInfo
	tstrM2mScanDone
	tstrM2mWifiscanResult
*/

bool thethingsio_disconnect_subscribe();


/* @typedef \
	tpfAppWifiCb

@brief
				

@param [in]	pvMsg
				A pointer to a buffer containing the notification parameters (if any). It should be
				Casted to the correct data type corresponding to the notification type.

@see
	tstrM2mWifiStateChanged
	tstrM2MWPSInfo
	tstrM2mScanDone
	tstrM2mWifiscanResult
*/
bool thethingsio_example_subscribe(char* topic);


bool thethingsio_activate_thing();

void thethingsio_set_thingtoken_state(int state);

int thethingsio_get_thingtoken_state();

void thethingsio_parsing_http_response_data(int response_code, char * response_data, int response_length);


#ifdef __cplusplus
}
#endif

#endif /* ATMEL_THETHINGSIO_H_ */