#include <ESP8266WiFi.h>
#include "SerialMenu.h"
#include "DataStream.h"
#include "secrets.h"

#define LED_PIN    5     
#define LED_COUNT  12     

SerialMenu::SerialMenu(String ssid, String password, String email, String emailPassword, String i)
: wifi(ssid,password,email,emailPassword)
{
  input = i;
}


MyWiFi& SerialMenu::getWiFi()
{
  return wifi;
}

void SerialMenu::printMainMenu()
{
  Serial.println("Choose one of the options below:");
  Serial.println("1-Enter SSID");
  Serial.println("2-Enter WI-FI Password");
  Serial.println("3-Enter Email");
  Serial.println("4-Enter Email Password");
  Serial.println("5-Connect with Wi-Fi"); 
  Serial.println("6-Start Program");
}



void SerialMenu::handleMainMenu()
{
  if (Serial.available() > 0) 
  { 
    String input = Serial.readStringUntil('\r');
    input.trim(); 

    if(input == "1")
    {    
      handleWifiSsidMenu();
    }
    else if(input == "2")
    {
      handleWifiPasswordMenu();
    }
    else if (input == "3")
    {
      handleEmailMenu();
    }
    else if (input == "4")
    {
      handleEmailPasswordMenu();
    }
    else if (input == "5") 
    {
      handleWiFiConnection();
    }
    else if (input == "6")
    {
      handleDataStream();
    }
    else if (input == "-help")
    {
      printMainMenu();
    }
    else
    {
      Serial.println("Unknown command. Write \"-help\" to see all command");
    }
  }  
}



void SerialMenu::handleWifiSsidMenu()
{ 

  int timer;
  int currtimer;
  timer = millis();
  currtimer = timer;
  int i = 0;

  Serial.println("Enter SSID. After 10 seconds without activity you will be returned to main menu");
  Serial.println("Enter q to leave");

  while((currtimer-timer) < 10000 && i == 0)
  {
    if(Serial.available() > 0)
    {
      String ssidInput = Serial.readStringUntil('\r');
      ssidInput.trim();
      if (ssidInput == "q")
      {
        Serial.println("You leave");
        Serial.println("");
        i++;
      }
      else
      {
      wifi.setSsid(ssidInput);
      Serial.println("Entered SSID: " + wifi.getSsid());
      Serial.println("");
      i++;
      }
    }
  }
   printMainMenu();
}



void SerialMenu::handleEmailMenu()
{
  int timer;
  int currtimer;
  timer = millis();
  currtimer = timer;
  int i = 0;

  Serial.println("Enter E-mail. After 10 seconds without activity you will be returned to main menu");
  Serial.println("Enter q to leave");

  while((currtimer-timer) < 10000 && i == 0)
  {
    if(Serial.available() > 0)
    {
      String emailInput = Serial.readStringUntil('\r');
      emailInput.trim();
      if (emailInput == "q")
      {
        Serial.println("You leave");
        Serial.println("");
        i++;
      }
      else
      {
      wifi.setEmail(emailInput);
      Serial.println("Entered E-mail:" + wifi.getEmail());
      i++;
      }
    }
  }
  printMainMenu();
}

void SerialMenu::handleEmailPasswordMenu()
{
  int timer;
  int currtimer;
  timer = millis();
  currtimer = timer;
  int i = 0;

  Serial.println("Enter password to your RCP account. After 10 seconds without activity you will be returned to main menu");
  Serial.println("Enter q to leave");

  while((currtimer-timer) < 10000 && i == 0)
  {
    if(Serial.available() > 0)
    {
      String accountPasswordInput = Serial.readStringUntil('\r');
      accountPasswordInput.trim();
      if (accountPasswordInput == "q")
      {
        Serial.println("You leave");
        i++;
      }
      else
      {
      wifi.setEmailPassword(accountPasswordInput);
      // Serial.println(wifi.getEmailPassword());                  /// Here you can change code to make print password after enterning the password
      // Serial.println("");                                       ///
      i++;
      }
    }
  }
  printMainMenu();
}

void SerialMenu::handleWifiPasswordMenu()
{
  int timer;
  int currtimer;
  timer = millis();
  currtimer = timer;
  int i = 0;

  Serial.println("Enter password to Wi-Fi. After 10 seconds without activity you will be returned to main menu");
  Serial.println("Enter q to leave");

  while((currtimer-timer) < 10000 && i == 0)
  {
    if(Serial.available() > 0)
    {
      String wifiPasswordInput = Serial.readStringUntil('\r');
      wifiPasswordInput.trim();
      if (wifiPasswordInput == "q")
      {
        Serial.println("You leave");
        i++;
      }
      else
      {
      wifi.setPassword(wifiPasswordInput);
      Serial.println("Entered password to Wi-Fi:" + wifi.getPassword());
      Serial.println("");
      i++;
      }
    }
  }
  printMainMenu();
}

void SerialMenu::handleWiFiConnection()
{
  WiFi.begin(WIFI_SSID ,WIFI_PASSWORD);  
  unsigned long start = millis();
  Serial.println("Connecting with Wi-Fi");
  Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
  while (WiFi.status() != WL_CONNECTED && millis()-start < 20000)
  {
    Serial.print(".");
    strip.begin();
    strip.show();
    strip.setBrightness(LED_BRIGHTNES);
    for (int i = 0; i < LED_COUNT; i++) 
    {
      strip.setPixelColor(i, strip.Color(255, 0, 0)); 
    }
    strip.show();
    delay(100);
    Serial.print(".");
    strip.begin();
    strip.show();
    strip.setBrightness(LED_BRIGHTNES);
    for (int i = 0; i < LED_COUNT; i++) 
    {
      strip.setPixelColor(i, strip.Color(0, 255, 0)); 
    }
    strip.show();
    delay(100);
    Serial.print(".");
    strip.begin();
    strip.show();
    strip.setBrightness(LED_BRIGHTNES);
    for (int i = 0; i < LED_COUNT; i++) 
    {
      strip.setPixelColor(i, strip.Color(0, 0, 255)); 
    }
    strip.show();
    delay(100);
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Done\n");
    strip.begin();
    strip.show();
    strip.setBrightness(LED_BRIGHTNES);
    for (int i = 0; i < LED_COUNT; i++) 
    {
      strip.setPixelColor(i, strip.Color(255, 255, 255)); 
    }
    strip.show();
  }
  else
  {    
    Serial.println("Done\n");
    strip.begin();
    strip.show();
    strip.setBrightness(LED_BRIGHTNES);
    for (int i = 0; i < LED_COUNT; i++) 
    {
      strip.setPixelColor(i, strip.Color(255, 0, 255)); 
    }
    Serial.println("Failed!");
  }
  // printMainMenu();  
}

void SerialMenu:: handleDataStream()
{
  if(WL_CONNECTED == WiFi.status())
  {
  DataStream stream(  wifi.getSsid(),wifi.getPassword() ,EMAIL_ADDRESS , EMAIL_PASSWORD ,"","","");  //     *** if you want to use menu in terminal: EMAIL_ADDRESS -> wifi.getEmail() , EMAIL_PASSWORD -> wifi.getEmailPassword() ***
  stream.rcpLoop();
  }
  else 
  {
  Serial.println("No Wi-Fi Connection. Operation is not allowed");
  printMainMenu();
  }
}
