#ifndef SERIALMENU_H
#define SERIALMENU_H


#include "MyWiFi.h"


class SerialMenu 
{
  public:
    MyWiFi wifi;

    SerialMenu(String ssid, String password, String email, String emailPassword, String i);

    MyWiFi& getWiFi();
    
    void printMainMenu();

    void handleMainMenu();
    void handleWifiSsidMenu();
    void handleWifiPasswordMenu();
    void handleEmailMenu();
    void handleEmailPasswordMenu();
    void handleWiFiConnection();
    void handleDataStream();
   
  private:

    String input;
};
#endif