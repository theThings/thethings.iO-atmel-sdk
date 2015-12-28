/**
 * \file
 *
 * \brief MAIN configuration.
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

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


// use this only during development, to avoid that new thing token is requested all the time
#define DEBUG_USE_THING_TOKEN_INSTEAD_OF_ACTIVATION_CODE
//#define DEBUG_THING_TOKEN							"A9hkXyJ_alXvvW8rjSao3Y3esnP2__AvQbSnEBnSRik"  // JOSEF Thing token
#define DEBUG_THING_TOKEN							"XJgU-PTj5k9VRvl_5I3O8mIkFNSYBpk7JMWRpQrNwQE"

// this activation code once requests a thing token. This thing token will then be stored in non volatile memory
#define MAIN_THETHINGSIO_ACTIVATION_CODE			"1wg1AcuFPUn9n8f2J9tBE0MJi1vvLVusYg"

// format to send values to thethings.io
#define MAIN_THETHINGSIO_JSON_START					"{\"values\":[{"
#define MAIN_THETHINGSIO_JSON_KEY_VALUE_SS			"\"key\":\"%s\",\"value\":\"%s\""
#define MAIN_THETHINGSIO_JSON_KEY_VALUE_SI			"\"key\":\"%s\",\"value\":\"%d\""
//#define MAIN_THETHINGSIO_JSON_KEY_VALUE_SMALL_SS	"\"%s\":\"%s\""						// minimum amount of characters for string-key and  string-value, returns error from thethings!!
//#define MAIN_THETHINGSIO_JSON_KEY_VALUE_SMALL_SI	"\"%s\":%d"							// minimum amount of characters for string-key and integer-value, returns error from thethings!!
#define MAIN_THETHINGSIO_JSON_SEPARATOR				","
#define MAIN_THETHINGSIO_JSON_END					"}]}"

#define MAIN_RES_HTTP_CODE_200						200			// "get" (request resource)
#define MAIN_RES_HTTP_CODE_201						201			// activate thing, "post" (create resource)
#define MAIN_RES_HTTP_CODE_400						400			// bad request
#define MAIN_RES_HTTP_CODE_500						500			// internal error


// power mode
#define MAIN_PS_SLEEP_MODE  M2M_NO_PS

#define HEX2ASCII(x) (((x) >= 10) ? (((x) - 10) + 'A') : ((x) + '0'))

//#define MAIN_M2M_AP_SEC							M2M_WIFI_SEC_WPA_PSK
//#define MAIN_M2M_AP_WEP_KEY						"WayraBarcelona"
//#define MAIN_M2M_AP_SSID_MODE					SSID_MODE_VISIBLE
// #define MAIN_M2M_DHCP_SERVER_IP					{192, 168, 1, 1}
	
	
/** security information for Wi-Fi connection */
//#define MAIN_WLAN_DEVICE_NAME           "WayraBCN" /**< Destination SSID */
//#define MAIN_WLAN_PSK					"WayraBarcelona"

//#define MAIN_WLAN_DEVICE_NAME           "mibamuseum2" /**< Destination SSID */
//#define MAIN_WLAN_PSK					"ciutatset1234"

#define MAIN_WLAN_DEVICE_NAME			"c6138a"
#define MAIN_WLAN_PSK					"271168418"


#define MAIN_WLAN_WEP_KEY_INDEX         1 /**< WEP key index */

// MQTT definitions

/* Max size of MQTT buffer. */
#define MAIN_MQTT_BUFFER_SIZE 128

#define MQTT_PORT 1883

#define MAIN_HTTP_PROV_SERVER_DOMAIN_NAME		"atmelconfig.com"

#define MAIN_M2M_DEVICE_NAME					"WINC1500_00:01"
#define MAIN_MAC_ADDRESS						{0xf8, 0xf0, 0x05, 0x45, 0xD4, 0x84}

/*static tstrM2MAPConfig gstrM2MAPConfig = {
	MAIN_M2M_DEVICE_NAME,
	1,
	0,
	WEP_40_KEY_STRING_SIZE,
	MAIN_M2M_AP_WEP_KEY,
	MAIN_M2M_AP_SEC,
	MAIN_M2M_AP_SSID_MODE,
	MAIN_M2M_DHCP_SERVER_IP
}; */



static CONST char gacHttpProvDomainName[] = MAIN_HTTP_PROV_SERVER_DOMAIN_NAME;

static uint8_t gau8MacAddr[] = MAIN_MAC_ADDRESS;
static int8_t gacDeviceName[] = MAIN_M2M_DEVICE_NAME;

static uint8_t gau8TheThingsIoThingToken[] = DEBUG_THING_TOKEN;

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H_INCLUDED */
