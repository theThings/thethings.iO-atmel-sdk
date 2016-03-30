## (Thethings.io cloud platorm and Atmel SAMD21XPLAINED PRO - WINC1500 client code)

This README is a technical description how to use thethings.iO with the Atmel studio WINC1500 client application.
Use as code base to create the firmware of a IOT product connected to thethings.IO using the SAMD21 cortex M0 and the WINC1500 Wifi chip.

The project implements the following features:

1. Activation code procedure (the code ask for the thing token using an activation code defined in the source code.)
2. READ a resource (using the HTTP protocol)
3. WRITE a resource  (using the HTTP protol)
4. SUBSCRIPTION (using the MQTT broker)

## Requirements

* Atmel SAMD21XPLAINED PRO board
* Atmel WINC 1500 module
* Atmel Studio 7.0
* Atmel Framework 
* Windows 7 SP1 (You can use a Windows in a virtual machine but the WINC1500 firmware upgrade ONLY works in a non virtualized Windows machine)
* thethings.iO free account
* A protoboard, cable connectors and a led to check the demo app.

This is the picture of the test project:
![Test project](https://raw.githubusercontent.com/theThings/thethings.iO-atmel-sdk/master/docs/atmel_pictures/samd21Xplained_winc1500.jpg)


## Code Description

### Activation Code 

change the Activation code in the thethingsio.h file

``` c
#define THETHINGSIO_MQTT_SUBSCRIPTION_ACTIVATE       1

// this activation code once requests a thing token. This thing token will then be stored in non volatile memory
// IMPORTANT !!!!!! you should modify the following line with your own activation code.
// Change the XXXXXX for your REAL activation code
#define MAIN_THETHINGSIO_ACTIVATION_CODE			"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" 
```


### Send data to thethings.io platform.


The example code sends in a interval the temperature register by the temperature sensor of the expansion IO board

```c
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
```

In the panel a similar graph is created with the values of the temperature sensor.

![Temperature the things io panel graph](https://raw.githubusercontent.com/theThings/thethings.iO-atmel-sdk/master/docs/atmel_pictures/temperature_panel.jpg)


### Recieve data in RealTime (with MQTT notifications)

The panel has an option to send test data to the thing.

![Send data notification](https://raw.githubusercontent.com/theThings/thethings.iO-atmel-sdk/master/docs/atmel_pictures/sending_data_from_panel.jpg)

set in thethingsio.h

``` c
#define THETHINGSIO_MQTT_SUBSCRIPTION_ACTIVATE       1
```
TO disable MQTT subscription set 0 to the THETHINGSIO_MQTT_SUBSCRIPTION_ACTIVATE define.

MQTT subscription need to have the always enable energy flag in the chipset.

The code to set the energy flag is in the main.c

```c

int main(void)
{
  ...
	/* setting sleep mode. */
	system_set_sleepmode(SYSTEM_SLEEPMODE_IDLE_0);
	...
	m2m_wifi_set_sleep_mode(MAIN_PS_SLEEP_MODE, 1);
	...
}
```


in the main.c there is a a register method to set the MQTT callback:

``` c
if (THETHINGSIO_MQTT_SUBSCRIPTION_ACTIVATE)
{
	/* Configure the MQTT subscribe broker */
	thethingsio_subscribe_config(thethingsio_subscribe_callback);
}
```

and define the callback method:

```c
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
```

### Before to Start

First task, you should upgrade the Atmel WiFi firmware. 

Search in example projects by WINC and select WINC_1500 Firmware upgrade Project (SAM21). 

Follow the firmware upgrade Project documentation. Reference at the end of the document.

Check how to start in the atmel oficial document that completes all the procedure to upgrade the wifi host driver interface necesary to allow to comunícate correctly from the chipset to the wifi chipset.

http://www.atmel.com/images/atmel-42417-getting-started-guide-for-atwinc1500wifi-using-samd21-xplained-pro_userguide.pdf

### About thethings.iO client library

thethings.iO [https://thethings.io] provides a client library to use with the  WINC1500. To download thethings.iO client code from: http://www.github.com/theThings/thethingsio-atmel-sdk

Download and open with the Atmel Studio 7. 

Go to File > Open > Project / Solutions. Select the Atmel Studio file on thethings.iO library folder.

Go to main.h file and configure the WiFi SSID with the user / password to connect to the WiFi AP.

The activation code is define in ACTION_CODE definition in main.h file. Use the thing token provide by thething.io platform at https://thethings.iO.

## FAQ

### How to debug the code?

With atmel studio you can debug using breakpoints and a variable editor.

### How can show the printf messages?

1. Download de realterm software.
2. Set the 115000 bauds/s and search the correct serial port number.
3. Connect 

![Realterm use](https://raw.githubusercontent.com/theThings/thethings.iO-atmel-sdk/master/docs/atmel_pictures/realterm.jpg)

### The wifi_cb doesn't execute the DHCP finish state.

The wifi_cb in the main.c is the callback method that you recieved from the wifi WINC1500 chipset. 

If wifi firmware is not the 19.3.0 version, you should upgrade the firmware.

To check the version, in the Realterm you should watch the following message:

(APP)(INFO)Chip ID 1503a0

(APP)(INFO)Firmware ver   : 19.3.0

(APP)(INFO)Min driver ver : 19.3.0

(APP)(INFO)Curr driver ver: 19.3.0

(APP)(INFO)POWER SAVE 0

Check how to start in the atmel oficial document that completes all the procedure to upgrade the wifi host driver interface necesary to allow to comunícate correctly from the chipset to the wifi chipset.

http://www.atmel.com/images/atmel-42417-getting-started-guide-for-atwinc1500wifi-using-samd21-xplained-pro_userguide.pdf

## Installation

1. Clone the github repository or Download the zip.
2. Open the solution with the Atmel Studio 7.0.
3. Install de EDG driver
4. Compile and deploy in the board.


## Contributions

Based in the Atmel ASF framework, thethings.io client code by Josef and Jordi Buges (Bedomo)

## License

Copyright [2016] [Thethings.IO]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.



