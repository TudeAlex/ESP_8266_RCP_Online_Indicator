#include <ESP8266WiFi.h>
#include "MyWiFi.h"




MyWiFi::MyWiFi(String s,String p,String e, String ep) 
 : ssid(s) , password(p), email(e), emailPassword(ep) {}


void MyWiFi::setSsid(String s)
{
  ssid = s;
}

void MyWiFi::setPassword(String p)
{
  password = p;
}

void MyWiFi::setEmail(String e)
{
  email = e;
}

void MyWiFi::setEmailPassword(String ep)
{
  emailPassword = ep;
}

String MyWiFi::getSsid()
{
  return ssid;
}

String MyWiFi::getPassword()
{
  return password;
}

String MyWiFi::getEmail()
{
  return email;
}

String MyWiFi::getEmailPassword()
{
  return emailPassword;
}
