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


## Code Modification

change the Activation code in the thethingsio.h file

``` c
#define THETHINGSIO_MQTT_SUBSCRIPTION_ACTIVATE       1

// this activation code once requests a thing token. This thing token will then be stored in non volatile memory
// IMPORTANT !!!!!! you should modify the following line with your own activation code.
// Change the XXXXXX for your REAL activation code
#define MAIN_THETHINGSIO_ACTIVATION_CODE			"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" 
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

### How to Start

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






## Motivation



## Installation

Provide code examples and explanations of how to get the project.

## API Reference

Depending on the size of the project, if it is small and simple enough the reference docs can be added to the README. For medium size to larger projects it is important to at least provide a link to where the API reference docs live.


## Contributors

Let people know how they can dive into the project, include important links to things like issue trackers, irc, twitter accounts if applicable.

## License

A short snippet describing the license (MIT, Apache, etc.)



