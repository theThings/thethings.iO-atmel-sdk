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

#include "debug_conf.h"
#include "thethingsio_client.h"
#include "tick_counter.h"


#define THETHINGSIO_EXAMPLE_ACTIVATION_CODE_LENGTH		34
#define THETHINGSIO_EXAMPLE_THING_TOKEN_LENGTH			43

// format to activate thing
#define THETHINGSIO_EXAMPLE_JSON_ACTIVATE			"{\"activationCode\":\"%s\"}"
	
#define THETHINGSIO_EXAMPLE_DUMMY_ACTIVATION_CODE	"0000000000000000000000000000000000"
#define THETHINGSIO_EXAMPLE_DUMMY_THING_TOKEN		"0000000000000000000000000000000000000000000"
#define THETHINGSIO_EXAMPLE_HTTP_CONTENT_TYPE		"application/json"
#define KEEP_ALIVE_VALUE   "?keepAlive=60000"
#define THETHINGSIO_EXAMPLE_HTTP_ACTIVATE_URL		"http://api.thethings.io/v2/things"
#define THETHINGSIO_EXAMPLE_HTTP_READ_AND_WRITE_URL "http://api.thethings.io/v2/things/" THETHINGSIO_EXAMPLE_DUMMY_THING_TOKEN
#define THETHINGSIO_EXAMPLE_HTTP_SUBSCRIBE			"v2/things/" THETHINGSIO_EXAMPLE_DUMMY_THING_TOKEN
#define MQTT_SUBCRIBE_TOPIC_NEW  "/v2/things" THETHINGSIO_EXAMPLE_DUMMY_THING_TOKEN


/** NVM Addresses */
//Current 1000, but Max is ((1023) * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE); taken from Parse example; NVMCTRL_PAGES is 1024, is total  number of pages
#define NVM_ADDR(x)							((1000 - x) * NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE)  // NVMCTRL_ROW_PAGES * NVMCTRL_PAGE_SIZE equals NVMCTRL_ROW_SIZE ??
#define NVM_ADDR_ROW_INTERVAL(x)			NVMCTRL_ROW_SIZE * x
#define NVM_ADDR_HOME_AP_INFO_SAVED			NVM_ADDR(1)
#define NVM_ADDR_THING_TOKEN_SAVED			NVM_ADDR(2)
#define NVM_ADDR_HOME_AP_SSID				NVM_ADDR(3)
#define NVM_ADDR_HOME_AP_SECTYPE			NVM_ADDR(4)
#define NVM_ADDR_HOME_AP_PASSWORD			NVM_ADDR(5)
#define NVM_ADDR_THING_TOKEN				NVM_ADDR(6)
//#define NVM_ADDR_FLUFFY_KEY_VALUE			NVM_ADDR_FLUFFY_KEY - NVM_ADDR_ROW_INTERVAL(MAX_KEY_COUNT)

// mqtt (TODO must remove) 
/* Max size of UART buffer. */
#define MAIN_CHAT_BUFFER_SIZE 64

/* Max size of MQTT buffer. */
#define MAIN_MQTT_BUFFER_SIZE 128

/* Limitation of user name. */
#define MAIN_CHAT_USER_NAME_SIZE 64

/* Chat MQTT topic. */
#define MAIN_CHAT_TOPIC "atmel/sample/chat_demo/"

// USER
char mqtt_user[64] = "";

// this flags indicate that if the MQTT broker is disconnect the application will start a new open MQTT broker session.
static bool gboolAlwaysMQTTConnected = true;

// locate this string in nvm, so thing token can be rewritten
static uint8_t gau8TheThingsiOHttpRWUrl[] = THETHINGSIO_EXAMPLE_HTTP_READ_AND_WRITE_URL;
static uint8_t gau8TheThingsiOActivationData[60] = {0, };
static uint8_t gau0TheThingsIOHttpSubsURL[] = THETHINGSIO_EXAMPLE_HTTP_SUBSCRIBE;

/** Instance of Timer module. */
struct sw_timer_module swt_module_inst;

/** Instance of HTTP client module. */
struct http_client_module http_client_module_inst;

struct http_client_module http_client_module_subscribe;

struct http_entity g_http_entity = {0,};
	
#define MQTT_SUBSCRIBE_TOPIC "v2/things/"

static const char main_mqtt_broker[] =  "mqtt.thethings.io";


/** Instance of Timer module. */
struct sw_timer_module swt_module_mqtt_inst;

/* Instance of MQTT service. */
static struct mqtt_module mqtt_inst;

/* MQTT buffer */
static char mqtt_buffer[MAIN_MQTT_BUFFER_SIZE];

/************************************************************************/
/* declare inner function                                               */
/************************************************************************/
static bool			_thethingsio_example_configure_http_client(thethingsio_http_cb cb);
static void			_thethingsio_example_configure_timer(void);

struct http_entity *_thethingsio_example_http_set_default_entity(void);
const char*			_thethingsio_example_http_get_contents_type(void *priv_data);
int					_thethingsio_example_http_get_contents_length(void *priv_data);
int					_thethingsio_example_http_read(void *priv_data, char *buffer, uint32_t size, uint32_t written);
void				_thethingsio_example_http_close(void *priv_data);

bool  thethingsio_example_http_init(thethingsio_http_cb cb)
{
	DEBUG(DEBUG_CONF_THETHINGSIO"thethingsio_example_init"DEBUG_EOL);
	/* Initialize the Timer. */
	_thethingsio_example_configure_timer();
	/* Initialize the HTTP client service. */
	if( !_thethingsio_example_configure_http_client(cb) )
	{
		DEBUG(DEBUG_CONF_THETHINGSIO"Error : thethingsio_example_init"DEBUG_EOL);
		return false;
	}
	return true;
}


/* check if thing has been configured already (thing token) */
uint8_t thethingsio_example_thing_token_available_nvm(void)
{
	uint8_t token_saved = false;
	enum status_code status = nvm_read_buffer(NVM_ADDR_THING_TOKEN_SAVED, &token_saved, 1);
	
	if(status != STATUS_OK) {
		printf("load thing token, read NVM error (0x%x) \r\n", status);
		return false;
	}
	
	if(token_saved != true) {
		printf("load thing token, thing token does not exist in NVM \r\n");
		return false;
	}
	return true;
}
/* if yes, load thing token from memory */
uint8_t thethingsio_example_load_thing_token_nvm(void) 
{
	/* check for valid thing token in NVM of device */
	uint8_t readBuffer_Temp[NVMCTRL_PAGE_SIZE] = { 0, };
	uint8_t readToken_Temp[NVMCTRL_PAGE_SIZE] = { 0, };
		
	enum status_code status = nvm_read_buffer(NVM_ADDR_THING_TOKEN, readBuffer_Temp, NVMCTRL_PAGE_SIZE);

	if(status != STATUS_OK) {
		printf("load thing token, read thing token error (0x%x)\r\n", status);
		return false;
	}
	
	
	// update thingsiO read write URL
	uint8_t thing_token_offset = sizeof(gau8TheThingsiOHttpRWUrl) - THETHINGSIO_EXAMPLE_THING_TOKEN_LENGTH - 1;
	
	memcpy((gau8TheThingsiOHttpRWUrl+thing_token_offset), readBuffer_Temp, THETHINGSIO_EXAMPLE_THING_TOKEN_LENGTH);	
	
	// update thingIO subscribe URL
	// add by jb to support subscription
	uint8_t things_token_subscribe_offset = sizeof(gau0TheThingsIOHttpSubsURL) - THETHINGSIO_EXAMPLE_THING_TOKEN_LENGTH -1;
	memcpy((gau0TheThingsIOHttpSubsURL+things_token_subscribe_offset), readBuffer_Temp, THETHINGSIO_EXAMPLE_THING_TOKEN_LENGTH);
	printf("subscribe thing token %s", readBuffer_Temp);
	printf("URL subscription %s"DEBUG_EOL, gau0TheThingsIOHttpSubsURL);
	// memcpy((gau0TheThingsIOHttpSubsURL+things_token_subscribe_offset + sizeof(KEEP_ALIVE_VALUE)), KEEP_ALIVE_VALUE, sizeof(KEEP_ALIVE_VALUE));
	
	
	return true;
}




/* if not, save new thing token to memory and use it */
uint8_t thethingsio_example_write_thing_token_nvm(char * thing_token)
{	
	uint8_t token_saved = true;
	uint8_t writeBuffer_Temp[NVMCTRL_PAGE_SIZE] = {0, };
		
	// erase flash memory area
	enum status_code status = nvm_erase_row(NVM_ADDR_THING_TOKEN_SAVED);
	if(status == STATUS_OK)
		status = nvm_erase_row(NVM_ADDR_THING_TOKEN);

	// rewrite flash memory area
	if(status == STATUS_OK)
		status = nvm_write_buffer(NVM_ADDR_THING_TOKEN_SAVED, &token_saved, 1);
	if(status == STATUS_OK)
		status = nvm_write_buffer(NVM_ADDR_THING_TOKEN, thing_token, THETHINGSIO_EXAMPLE_THING_TOKEN_LENGTH);
	
	// check if all the operations went through correctly
	if(status != STATUS_OK)
		printf("set thing token, write thing token error!!\r\n");
	
	// update thingsiO read write URL
	uint8_t thing_token_offset = sizeof(gau8TheThingsiOHttpRWUrl) - THETHINGSIO_EXAMPLE_THING_TOKEN_LENGTH - 1;
	memcpy((gau8TheThingsiOHttpRWUrl+thing_token_offset),thing_token, THETHINGSIO_EXAMPLE_THING_TOKEN_LENGTH);
	
	return true;
}

uint8_t thethingsio_create_mqtt_subscribe_broker(char *thing_token)
{
	printf("create mqtt broker"DEBUG_EOL);
	
	
	printf("end mqtt broker"DEBUG_EOL);
}


bool thethingsio_example_activate_thing(char * activation_code)
{
	bool ret = false;
	int err_code = 0;
	DEBUG(DEBUG_CONF_THETHINGSIO"thethingsio_example_activiate"DEBUG_EOL);
	
	sprintf(gau8TheThingsiOActivationData,THETHINGSIO_EXAMPLE_JSON_ACTIVATE,activation_code);
	
	struct http_entity * entity = _thethingsio_example_http_set_default_entity();
	entity->priv_data = (void*)gau8TheThingsiOActivationData;
	
	err_code = http_client_send_request(&http_client_module_inst, THETHINGSIO_EXAMPLE_HTTP_ACTIVATE_URL,HTTP_METHOD_POST,entity, NULL);
	if( err_code == 0)
	{
		ret = true;	
		tick_counter_pending_timer();
	}
	else
	{
		DEBUG(DEBUG_CONF_THETHINGSIO"Error : thethingsio_example_activiate code %d"DEBUG_EOL, err_code);
	}
	
	return ret;
}

bool  thethingsio_example_subscribe(char *topic)
{
	
	bool ret = false;
	int err_code = 0;
	struct http_entity * entity = _thethingsio_example_http_set_default_entity();
	DEBUG(DEBUG_CONF_THETHINGSIO"thethingsio_example_subscribe"DEBUG_EOL);

}

bool	thethingsio_example_read_and_write(char * write_data)
{
	bool ret = false;
	int err_code = 0;
	char ext_header[100] = {0,};
	struct http_entity * entity = _thethingsio_example_http_set_default_entity();
	
	DEBUG(DEBUG_CONF_THETHINGSIO"thethingsio_example_read_and_write"DEBUG_EOL);
	
	entity->priv_data = (void*)write_data;	
	//sprintf(ext_header,THETHINGSIO_EXAMPLE_HTTP_CIK_EXT_HEADER, cik);	
	
	err_code = http_client_send_request(&http_client_module_inst, gau8TheThingsiOHttpRWUrl,HTTP_METHOD_POST,entity, NULL);
	
	
	if( err_code == 0)
	{
		ret = true;		
		tick_counter_pending_timer();
	}
	else
	{
		DEBUG(DEBUG_CONF_THETHINGSIO"Error : thethingsio_example_read_and_write code %d"DEBUG_EOL, err_code);
	}
	return ret;
}



/**
 * \brief Configure Timer module.
 */
static void _thethingsio_example_configure_timer(void)
{
	struct sw_timer_config swt_conf;
	sw_timer_get_config_defaults(&swt_conf);

	sw_timer_init(&swt_module_inst, &swt_conf);
	sw_timer_enable(&swt_module_inst);
}


static void _thethingsio_mqtt_configure_timer(void)
{
	
	struct sw_timer_config swt_conf_mqtt;
	sw_timer_get_config_defaults(&swt_conf_mqtt);

	sw_timer_init(&swt_module_mqtt_inst, &swt_conf_mqtt);
	sw_timer_enable(&swt_module_mqtt_inst);
	
}

/**
 * \brief Configure HTTP client module.
 */
static bool _thethingsio_example_configure_http_client(thethingsio_http_cb cb)
{
	struct http_client_config httpc_conf;
	int ret;

	http_client_get_config_defaults(&httpc_conf);

	// httpc_conf.recv_buffer_size = 256;
	httpc_conf.recv_buffer_size = 512;
	httpc_conf.send_buffer_size = 1024;
	httpc_conf.timer_inst = &swt_module_inst;
	/* ipinfo.io send json format data if only client is a curl. */
	httpc_conf.user_agent = "curl/7.10.6";

	ret = http_client_init(&http_client_module_inst, &httpc_conf);
	if (ret < 0) {
		DEBUG(DEBUG_CONF_THETHINGSIO"HTTP client initialization has failed(%s)"DEBUG_EOL, strerror(ret));
		return false;
	}
	

	http_client_register_callback(&http_client_module_inst, cb);
	return true;
}

struct http_entity * _thethingsio_example_http_set_default_entity()
{
	memset(&g_http_entity, 0x00, sizeof(struct http_entity));
	g_http_entity.close = _thethingsio_example_http_close;
	g_http_entity.is_chunked = 0;
	g_http_entity.priv_data = NULL;
	g_http_entity.read = _thethingsio_example_http_read;
	g_http_entity.get_contents_length = _thethingsio_example_http_get_contents_length;
	g_http_entity.get_contents_type = _thethingsio_example_http_get_contents_type;
	
	return &g_http_entity;
}

const char* _thethingsio_example_http_get_contents_type(void *priv_data)
{
	return (const char*)THETHINGSIO_EXAMPLE_HTTP_CONTENT_TYPE;
}

int _thethingsio_example_http_get_contents_length(void *priv_data)
{
	return strlen( (char*)priv_data);
}

int _thethingsio_example_http_read(void *priv_data, char *buffer, uint32_t size, uint32_t written)
{
	int32_t length = 0;
	
	if(priv_data)
	{
		length = strlen( (char*)priv_data);
		memcpy(buffer,(char*)priv_data, length);
	}
	
	return length;
}


static void mqtt_callback(struct mqtt_module *module_inst, int type, union mqtt_data *data)
{
	printf("MQTT CALLBACK ENTER"DEBUG_EOL);
	switch (type) {
	case MQTT_CALLBACK_SOCK_CONNECTED:
	{
		printf("MQTT SOCK CONNECTED"DEBUG_EOL);
		/*
		 * If connecting to broker server is complete successfully, Start sending CONNECT message of MQTT.
		 * Or else retry to connect to broker server.
		 */
		if (data->sock_connected.result >= 0) {
			printf("mqtt connect broker"DEBUG_EOL);
		    printf("socket %i", data->sock_connected.result);		
			int response = mqtt_connect_broker(module_inst, 1, NULL, NULL, gau0TheThingsIOHttpSubsURL, NULL, NULL, 0, 0, 0);
			// int response = mqtt_connect_broker(module_inst, 1, "1", "test", NULL, NULL, NULL, 0, 0, 0);
			printf("response %s %i"DEBUG_EOL,gau0TheThingsIOHttpSubsURL, response);
		} else {
			printf("Connect fail to server(%s)! retry it automatically.\r\n", main_mqtt_broker);
			mqtt_connect(module_inst, main_mqtt_broker); /* Retry that. */
		}
	}
	break;

	case MQTT_CALLBACK_CONNECTED:
		if (data->connected.result == MQTT_CONN_RESULT_ACCEPT) {
			/* Subscribe chat topic. */
			/* the subscribe topic in the subscribe endpoint */
			printf("subscribe topic %s"DEBUG_EOL, gau0TheThingsIOHttpSubsURL);
			mqtt_subscribe(module_inst, gau0TheThingsIOHttpSubsURL, 0);
			/* Enable USART receiving callback. */
			
			// TODO we have mqtt connection
			//usart_enable_callback(&cdc_uart_module, USART_CALLBACK_BUFFER_RECEIVED);
			printf("Preparation of the chat has been completed.\r\n");
		} else {
			/* Cannot connect for some reason. */
			printf("MQTT broker decline your access! error code %d\r\n", data->connected.result);
		}

		break;

	case MQTT_CALLBACK_RECV_PUBLISH:
		printf("recieve publish"DEBUG_EOL);
		/* You received publish message which you had subscribed. */
		if (data->recv_publish.topic != NULL && data->recv_publish.msg != NULL) {
				for (int i = 0; i < data->recv_publish.msg_size; i++) {
					printf("%c", data->recv_publish.msg[i]);
				}
				
				
			
			/* printf("%c", data->recv_publish.topic[i]);
				printf("\r\n");
			if (!strncmp(data->recv_publish.topic, MAIN_CHAT_TOPIC, strlen(MAIN_CHAT_TOPIC))) {
				///* Print user name and message
				for (int i = strlen(MAIN_CHAT_TOPIC); i < data->recv_publish.topic_size; i++) {
					printf("%c", data->recv_publish.topic[i]);
				}
				printf(" >> ");
				for (int i = 0; i < data->recv_publish.msg_size; i++) {
					printf("%c", data->recv_publish.msg[i]);
				}
				printf("\r\n");
			} */
		}

		break;

	case MQTT_CALLBACK_DISCONNECTED:
		/* Stop timer and USART callback. */
		printf("MQTT disconnected\r\n");
		
		if (gboolAlwaysMQTTConnected)
		{	
			mqtt_connect(module_inst, main_mqtt_broker);
		}
		
		// TODO we don't have mqtt connection
		//usart_disable_callback(&cdc_uart_module, USART_CALLBACK_BUFFER_RECEIVED);
		break;
	}
}

static void configure_mqtt(void)
{
	printf("configure mqtt"DEBUG_EOL);
	struct mqtt_config mqtt_conf;
	int result;

	mqtt_get_config_defaults(&mqtt_conf);
	/* To use the MQTT service, it is necessary to always set the buffer and the timer. */
	mqtt_conf.timer_inst = &swt_module_mqtt_inst;
	mqtt_conf.recv_buffer = mqtt_buffer;
	mqtt_conf.recv_buffer_size = MAIN_MQTT_BUFFER_SIZE;

	result = mqtt_init(&mqtt_inst, &mqtt_conf);
	if (result < 0) {
		printf("MQTT initialization failed. Error code is (%d)\r\n", result);
		while (1) {
		}
	}

	result = mqtt_register_callback(&mqtt_inst, mqtt_callback);
	if (result < 0) {
		printf("MQTT register callback failed. Error code is (%d)\r\n", result);
		while (1) {
		}
	}
	
	printf("end configure mqtt"DEBUG_EOL);
}

bool thethingsio_subscribe_config(void)
{
	printf("subscribe config"DEBUG_EOL);
	// configure a special timer for the mqtt infraestructure
	_thethingsio_mqtt_configure_timer();
		
	configure_mqtt();
	
	return true;
}


bool thethingsio_connect_subscribe()
{
	printf("connect subscribe"DEBUG_EOL);
	
	
	printf("mqtt broker: %s"DEBUG_EOL, main_mqtt_broker);
    mqtt_connect(&mqtt_inst, main_mqtt_broker);	
}


bool thethingsio_refresh_subscribe()
{
	

}

bool thethingsio_disconnect_subscribe()
{
	
}

void _thethingsio_example_http_close(void *priv_data)
{
}