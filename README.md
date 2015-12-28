# thethingsio-atmel-sdk

This README is a technical description how to use thethings.iO with the Atmel studio WINC1500 client application.

## Requirements

* Atmel SAMD21XPLAINED PRO board
* Atmel Studio 7.0
* Atmel Framework 
* Windows 7 SP1 (You can use a Windows in a virtual machine but the WINC1500 firmware upgrade only Works in a non virtualized Windows machine).
* thethings.iO free account

## How to Start

First task, you should upgrade the Atmel WiFi firmware. 

Search in example projects by WINC and select WINC_1500 Firmware upgrade Project (SAM21). 

Follow the firmware upgrade Project documentation. Reference at the end of the document.

Check how to start in the atmel oficial document that completes all the procedure to upgrade the wifi host driver interface necesary to allow to comunícate correctly from the chipset to the wifi chipset.

http://www.atmel.com/images/atmel-42417-getting-started-guide-for-atwinc1500wifi-using-samd21-xplained-pro_userguide.pdf


## About thethings.iO client library

thethings.iO [https://thethings.io] provides a client library to use with the  WINC1500. To download thethings.iO client code from: http://www.github.com/theThings/thethingsio-atmel-sdk

Download and open with the Atmel Studio 7. 

Go to File > Open > Project / Solutions. Select the Atmel Studio file on thethings.iO library folder.

Go to main.h file and configure the WiFi SSID with the user / password to connect to the WiFi AP.

The activation code is define in DEBUG_THING_TOKEN in main.h file. Use the thing token provide by thething.io platform at https://thethings.iO.



