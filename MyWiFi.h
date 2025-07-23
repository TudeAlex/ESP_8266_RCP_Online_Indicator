
#ifndef MYWIFI_H
#define MYWIFI_H

#include <Arduino.h>



class MyWiFi 
{
  
  private:
    String ssid;
    String password;
    String email;
    String emailPassword;

  public:
    MyWiFi(String s,String p,String e,String ep);
    
    void setSsid(String s);
    String getSsid();

    void setPassword(String p);
    String getPassword();

    void setEmail(String e);
    String getEmail();

    void setEmailPassword(String ep);
    String getEmailPassword();
};

#endif
