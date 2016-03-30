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


// power mode
// The M2M_NO_PS mode don't switch off the WIFI 
#define MAIN_PS_SLEEP_MODE  M2M_NO_PS

#define HEX2ASCII(x) (((x) >= 10) ? (((x) - 10) + 'A') : ((x) + '0'))

// This define the user and password for connect to the wifi infraestructure / Changue for your own
// this is the GUEST thethings io network if you will be in our office you can use it ;)
#define MAIN_WLAN_DEVICE_NAME			 "thethings.iO - GUEST"
#define MAIN_WLAN_PSK					 "ilovethethingsioguests"


#define MAIN_WLAN_WEP_KEY_INDEX         1 /**< WEP key index */

#define MAIN_HTTP_PROV_SERVER_DOMAIN_NAME		"atmelconfig.com"

#define MAIN_M2M_DEVICE_NAME					"WINC1500_00:01"
#define MAIN_MAC_ADDRESS						{0xf8, 0xf0, 0x05, 0x45, 0xD4, 0x84}

// DATA Struct for WEP connections
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


#ifdef __cplusplus
}
#endif

#endif /* MAIN_H_INCLUDED */
