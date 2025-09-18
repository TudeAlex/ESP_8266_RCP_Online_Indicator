#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>
#include "MyWiFi.h"
#include "SerialMenu.h"
#include "secrets.h"


#define LED_PIN    5     
#define LED_COUNT  12     



SerialMenu menu("","","","","");


void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(20000000); 
  menu.handleWiFiConnection();
}


void loop()
{   
    menu.handleDataStream();  //  *** commend it if you want to use menu in terminal ***
//    menu.handleMainMenu();  //  *** uncommend if you want to use menu in terminal ***
}