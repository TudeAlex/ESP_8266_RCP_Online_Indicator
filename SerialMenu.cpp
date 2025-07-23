#include "core_esp8266_features.h"
#include <ESP8266WiFi.h>
#include "SerialMenu.h"
#include "DataStream.h"
#include "secrets.h"



SerialMenu::SerialMenu(String ssid, String password, String email, String emailPassword, String i)
: wifi(ssid,password,email,emailPassword)
{

  input = i;
}

        /// Metody powiazane z polem obiektu klasy MyWIFI
MyWiFi& SerialMenu::getWiFi()
{
  return wifi;
}

// void SerialMenu::echoInput()
// {
//   String c;
//   while(c != "\r")
//   {
//   if(Serial.available())
//   {
//     char c = Serial.read();
//     Serial.write(c);
//   }
//   }
// }
        /// Metody obslugi menu
void SerialMenu::printMainMenu()
{
  Serial.println("Choose one of the options below:");
  // Serial.println("Options:");
  Serial.println("1-Enter SSID");
  Serial.println("2-Enter WI-FI Password");
  Serial.println("3-Enter Email");
  Serial.println("4-Enter Email Password");
  Serial.println("5-Connect with Wi-Fi"); 
  Serial.println("6-Start Program");
}



void SerialMenu::handleMainMenu()
{

  // handleWiFiConnection();
  // handleDataStream();

  if (Serial.available() > 0) 
  { 
    // echoInput();
    String input = Serial.readStringUntil('\r');
    input.trim();
    // Serial.println(input);  
    if(input == "1")
    {    
      handleWifiSSIDMenu();
    }
    else if(input == "2")
    {
      handleWifiPASSMenu();
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



void SerialMenu::handleWifiSSIDMenu()
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
      // echoInput();
      String ssidInput = Serial.readStringUntil('\r');
      ssidInput.trim();
      if (ssidInput == "q")
      {
        Serial.println("You leave");
        i++;
      }
      else
      {
      wifi.setSsid(ssidInput);
      Serial.println("Ssid:" + wifi.getSsid());
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

  Serial.println("Enter email. After 10 seconds without activity you will be returned to main menu");
  Serial.println("Enter q to leave");

  while((currtimer-timer) < 10000 && i == 0)
  {
    if(Serial.available() > 0)
    {
      // echoInput();
      String passwordInput = Serial.readStringUntil('\r');
      passwordInput.trim();
      if (passwordInput == "q")
      {
        Serial.println("You leave");
        i++;
      }
      else
      {
      wifi.setEmail(passwordInput);
      Serial.println("Password:" + wifi.getEmail());
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

  Serial.println("Enter email. After 10 seconds without activity you will be returned to main menu");
  Serial.println("Enter q to leave");

  while((currtimer-timer) < 10000 && i == 0)
  {
    if(Serial.available() > 0)
    {
      // echoInput();
      String passwordInput = Serial.readStringUntil('\r');
      passwordInput.trim();
      if (passwordInput == "q")
      {
        Serial.println("You leave");
        i++;
      }
      else
      {
      wifi.setEmailPassword(passwordInput);
      // Serial.println(wifi.getEmailPassword());
      i++;
      }
    }
  }
  printMainMenu();
}

void SerialMenu::handleWifiPASSMenu()
{
  int timer;
  int currtimer;
  timer = millis();
  currtimer = timer;
  int i = 0;

  Serial.println("Enter WiFI. After 10 seconds without activity you will be returned to main menu");
  Serial.println("Enter q to leave");

  while((currtimer-timer) < 10000 && i == 0)
  {
    if(Serial.available() > 0)
    {
      // echoInput();
      String passwordInput = Serial.readStringUntil('\r');
      passwordInput.trim();
      if (passwordInput == "q")
      {
        Serial.println("You leave");
        i++;
      }
      else
      {
      wifi.setPassword(passwordInput);
      Serial.println("Password:" + wifi.getPassword());
      i++;
      }
    }
  }
  printMainMenu();
}

void SerialMenu::handleWiFiConnection()
{
  WiFi.begin(WIFI_SSID ,WIFI_PASSWORD); //  "RND-GUEST","RND-GUEST"
  // WiFiClientSecure client;
  // client.setInsecure();
  unsigned long start = millis();
  Serial.println("Connecting with WI-FI");
  while (WiFi.status() != WL_CONNECTED && millis()-start < 20000)
  {
    Serial.print(".");
    delay(100);
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Done\n");
  }
  else
  {
    Serial.println("Failed!");
  }
  // printMainMenu();  
}

void SerialMenu:: handleDataStream()
{
  if(WL_CONNECTED == WiFi.status())
  {
  DataStream stream(  wifi.getSsid(),wifi.getPassword() ,EMAIL_ADDRESS , EMAIL_PASSWORD ,"","","");  //   "RND-GUEST","RND-GUEST"  wifi.getSsid() wifi.getPassword()  wifi.getEmail() wifi.getEmailPassword()
  stream.rcpLoop();
  }
  else 
  {
  Serial.println("No WIFI Connection. Operation is not allowed");
  printMainMenu();
  }
}
