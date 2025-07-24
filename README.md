  #ESP_8266_RCP_Online_Indicator 

This project is designet to signal presence status with LED, using request to https://panel.rcponline.pl.

## Author

This project was developed by a student intern during a summer internship, with the aim of gaining practical experience in embedded systems and network communication.

Created as part of a university internship – this is not a commercial product.


## HOW_TO_USE_IT

To use Code you must enter SSID and pasword of your wifi, and email and password to your RCP account to  'secrets.cpp'.  
Next upload 'RCP_Project_Public.ino' to your ESP8266. All of the files in this repository (without README obviusly), must be in same folder named 'RCP_Project_Public' (you can change it but it must be the same as .ino file). 
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

## USING_WITH_TERMINAL_MENU

device can be used with menu in terminal, but you 

## HOW_TO_BUILD_IT

The installation instructions for the device and the LED module will be included (or not).

## IMPORTANT

Code was used and tested only with Polish version of website, to make it work for English version it is necessary to modify the methods extractPresenceStatus() and ledDriver

## ADDING_FEATERS

Initially, the device was to be operated by the user from the menu, so there are methods in code that allows you use device with control menu
