#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include "MyWiFi.h"
#include "SerialMenu.h"
#include "secrets.h"


SerialMenu menu("","","","","");



unsigned long lastRequestTime = 0;
unsigned long lastRefreshTime = 0; 
const unsigned long requestInterval = 3000; // 10sekund
const unsigned long refreshInterval = 60000; // 20minut
int accessTokenValue = 0;
int employeeEvent = 0;

void setup()
{
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.setTimeout(20000000); // TODO/FIXME
  menu.handleWiFiConnection();
  digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(14,HIGH);
  digitalWrite(15,HIGH);

}


void loop()
{    
    menu.handleDataStream();
}