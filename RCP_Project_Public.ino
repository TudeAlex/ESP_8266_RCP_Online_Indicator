#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include "MyWiFi.h"
#include "SerialMenu.h"
#include "secrets.h"


SerialMenu menu("","","","","");


void setup()
{
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.setTimeout(20000000); 
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(14,HIGH);
  digitalWrite(15,HIGH);
  menu.handleWiFiConnection();
}


void loop()
{   
    menu.handleDataStream();  //  *** commend it if you want to use menu in terminal ***
//    menu.handleMainMenu();  //  *** uncommend it if you want to use menu in terminal ***
}