#include "WString.h"
#ifndef DATASTREAM_H
#define DATASTREAM_H

#include "MyWiFi.h"
#include <Adafruit_NeoPixel.h>


class DataStream 
{
  public:
    MyWiFi wifi_stream;


    DataStream(String ssid, String password, String email, String emailPassword, String status, String cookie, String csrf);


    MyWiFi& getWiFi();
 

    String getStatus();
    void setStatus(String i);

    String getCookie();
    void setCookie(String c);

    String getCsrf();
    void setCsrf(String c);
 
    void rcpLoop();

    void ledDriver();

    void login();
    void login_check();
    void getMyStatus();
    void two_auth_check();
    void app();
    int getMyStatus0();
    void clockEventBreak();
    String extractPresenceStatus(String i);
    
    String gettingCsrf(uint8_t* b);
    String gettingCookie(String b);
    String decodeUnicode(String i);


    void handleEmployerStatus();
    
  private:

    String ssid;
    String password;
    String email;
    String emailPassword;
    String status;
    String cookie;
    String csrf;
};
#endif