  #ESP_8266_RCP_Online_Indicator 

This project is designet to signal presence status with LED, using request to https://panel.rcponline.pl.

## Author

This project was developed by a student intern during a summer internship, with the aim of gaining practical experience in embedded systems and network communication.

Created as part of a university internship – this is not a commercial product.


## HOW_TO_USE_IT

Use Arduino IDE to setup this device.
To use Code you must enter SSID and pasword of your wifi, and email and password to your RCP account to  'secrets.cpp'.  
Next upload 'RCP_Project_Public.ino' to your ESP8266. All of the files in this repository (without README obviusly), must be in same folder named 'RCP_Project_Public' (you can change it but it must be the same as .ino file). To ensure proper operation of the device, all libraries listed in the `LIBRARIES` section must be installed in the Arduino IDE.
There is 4 types of presence status, if device succesfuly logged into your account, the device will set the designated outputs to high depending on the status:
For programmers: 
- **On site** output 12
- **On break** output 13
- **On business exit** output 14
- **Away** output 15
For user:
- **On site** D6
- **On break** D7
- **On business exit** D5
- **Away** D8
It is recommended to use colors of LEDs corresponding to the colors assigned to statuses on the RCP website.
- **On site** Green
- **On break** Blue
- **On business exit** Yellow
- **Away** Red 

If connected correctly, at the beggining all LED will be turned on till first succesful request GetMyStatus0 (request that gets status of user). If nothing changes, that mean there is problem with connection. Firstly restart device by using button 'rst' on ESP8266 if there is problem with device.
## LIBRARIES

Libraries used for project:

- **ESP8266WiFi.h**
- **WiFiClientSecure.h**
- **ESP8266HTTPClient.h**

## IMPORTANT

Code was used and tested only with Polish version of website, to make it work for English version it is necessary to modify the methods extractPresenceStatus() and ledDriver


## ADDING_FEATERS

Functions related to retrieving additional data from the RCP panel can be implemented by modifying the `externPresenceStatus` method. This method handles the response from the server when requesting the endpoint `GetMyStatus0`.

The response message contains at least the following additional information:
- **Total work time**
- **Time of the last activity**
- **Last Event**
To perform different actions during the login or status-check process, you can also create a modified version of the `getMyStatus0()` method. Its parameters should match those required for the desired request.

## USING_WITH_TERMINAL_MENU

The menu usage is guided by in-code comments marked with <pre> ``` *** comment *** ``` </pre>. Follow these annotations step by step to ensure proper operation.



