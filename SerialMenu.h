#ifndef SERIALMENU_H
#define SERIALMENU_H

#include <Arduino.h>
#include "MyWiFi.h"


class SerialMenu 
{
  public:
    MyWiFi wifi;

    /// Konstruktor
    SerialMenu(String ssid, String password, String email, String emailPassword, String i);
    ///



    /// Metody Pola wifi
    MyWiFi& getWiFi();
    ///

    ///Metody obslugi Menu
    void echoInput();
    void printMainMenu();

    void handleMainMenu();
    void handleWifiSSIDMenu();
    void handleWifiPASSMenu();
    void handleEmailMenu();
    void handleEmailPasswordMenu();
    void handleWiFiConnection();
    void handleDataStream();
    ///

    //Pole pole 
  private:

    String input;
};
#endif