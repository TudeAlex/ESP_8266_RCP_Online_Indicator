#ifndef SERIALMENU_H
#define SERIALMENU_H

#include <Arduino.h>
#include "MyWiFi.h"


class SerialMenu 
{
  public:
    MyWiFi wifi;

    SerialMenu(String ssid, String password, String email, String emailPassword, String i);

    MyWiFi& getWiFi();
    
    void echoInput();
    void printMainMenu();

    void handleMainMenu();
    void handleWifiSSIDMenu();
    void handleWifiPASSMenu();
    void handleEmailMenu();
    void handleEmailPasswordMenu();
    void handleWiFiConnection();
    void handleDataStream();
   
  private:

    String input;
};
#endif