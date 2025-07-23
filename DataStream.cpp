#include "HardwareSerial.h"
#include "core_esp8266_features.h"
#include <ESP8266WiFi.h>
#include "SerialMenu.h"
#include "MyWiFi.h"
#include "DataStream.h"
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

DataStream::DataStream(String ssid, String password, String email,String emailPassword, String status, String cookie, String csrf)
: wifi_stream(ssid,password,email,emailPassword),status(status), cookie(cookie), csrf(csrf) {}

MyWiFi& DataStream::getWiFi()
{
  return wifi_stream;
}

String DataStream:: getStatus()
{
  return status;
}

void DataStream:: setStatus(String s)
{
  status = s;
}

String DataStream:: getCookie()
{
  return cookie;
}

void DataStream:: setCookie(String c)
{
  cookie = c;
}

String DataStream:: getCsrf()
{
  return csrf;
}

void DataStream:: setCsrf(String c)
{
  csrf = c;
}

void DataStream:: rcpLoop()
{
  Serial.println("Enter q to leave");
  int i = 0;  
  login();
  login_check();
  two_auth_check();
  delay(5000);
  app();

  while(i == 0 )
  {
    if(Serial.available() > 0)
    {
      String quitInput = Serial.readStringUntil('\r');
      if(quitInput = "q")
      {
        i++;
        setStatus("0");
      }
      else {}
    }
    else 
    {
      getMyStatus0();
      delay(500);
      ledDriver();
      delay(10000);   
    }

  }

}

void DataStream:: login()
{
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure(); 

  http.begin(client , "https://panel.rcponline.pl/login/");
  http.addHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36");
  
  const char* keys[] = {"Set-Cookie"};
  http.collectHeaders(keys,1);

  int httpCode = http.GET();

  bool stateOfHeader = http.hasHeader("Set-Cookie");
  Serial.println(stateOfHeader);
  String csrf = "0";
    if (httpCode>0)
    {
      Serial.print("\n\rlogin :");
      Serial.println(httpCode);
      if (httpCode == HTTP_CODE_OK)
      {
        int len = http.getSize();

        uint8_t buff[512] = { 0 };

#if 0
        Serial.println(http.getString());
#else 
        
        WiFiClient* stream = &client;


        while (http.connected() && (len > 0 || len == -1))
        {

          int c = stream->readBytes(buff, std::min((size_t)len, sizeof(buff)));

          if (!c)
          {
            Serial.println("read timeout");
          }

          if (csrf == "0")
          {
            csrf = gettingCsrf(buff);
            
          }
          if (len > 0) {len -= c;}
        }
        String headerValue = http.header("Set-Cookie");

#endif
        setCookie(gettingCookie(headerValue));
        setCsrf(csrf);
      }
    } 
}

void DataStream:: ledDriver()
{
  String employeeEvent = getStatus();
  employeeEvent.trim();
  Serial.println(employeeEvent);
  if (employeeEvent == "1" )
  {
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);
  }
  else if (employeeEvent == "2")
  {
    digitalWrite(12,LOW);
    digitalWrite(13,HIGH);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);
  }
  else if (employeeEvent == "3" )
  {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    digitalWrite(14,HIGH);
    digitalWrite(15,LOW);
  }
  else if (employeeEvent == "4" )
  {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    digitalWrite(14,LOW);
    digitalWrite(15,HIGH);    
  }

  delay(5);
}

String DataStream:: gettingCsrf(uint8_t* b)
{
  String text = String((char*)b);
  const String tag_csrf = "<input type=\"hidden\" name=\"_csrf_token\" id=\"_csrf_token\" " ; 

  String founded_csrf = "";

  int start_tag_position = text.indexOf(tag_csrf);

  if (start_tag_position != -1)
  {
    int start_quotation_mark = text.indexOf('value="',start_tag_position);
    int end_quotation_mark = text.indexOf('" />',start_quotation_mark +1);
    if (start_quotation_mark != -1 && end_quotation_mark !=-1)
    {
      String csrf_text = text.substring(64 + start_quotation_mark, end_quotation_mark -3);
      return csrf_text;
    }
    else
    {
      return "0";
    }
  }
  else 
  {
    return "0";
  }
}

String DataStream:: gettingCookie(String b)
{
String tekst = b;
  
  const String tag_Cookie = "PHPSESSID=";
  const String tag_End_Cookie = "; expires";
  
  String znalezione_slowo = "";

  int start_pozycja_tagu = tekst.indexOf(tag_Cookie);

  if (start_pozycja_tagu != -1) 
  {
    int start_cudzyslowu = tekst.indexOf(tag_Cookie, start_pozycja_tagu);
    int koniec_cudzyslowu = tekst.indexOf(tag_End_Cookie, start_cudzyslowu + 1);

    if (start_cudzyslowu != -1 && koniec_cudzyslowu != -1) 
    {
      String cookie_text = tekst.substring(start_pozycja_tagu+10,koniec_cudzyslowu);
      return cookie_text;
    }
  }
  if (znalezione_slowo.length() > 0) {
  }
    return "0";
}

void DataStream:: login_check()
{
  String payload = "_username=" + wifi_stream.getEmail() +
                   "&_password=" + wifi_stream.getEmailPassword() +
                   "&_csrf_token=" + getCsrf();

  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure(); 

  http.begin(client , "https://panel.rcponline.pl/login_check/");
  http.addHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36");
  http.addHeader("Cookie", "PHPSESSID=" + getCookie());
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Referer" , "https://panel.rcponline.pl/login/");

  const char* keys[] = {"Set-Cookie","Set-Cookie"};
  http.collectHeaders(keys,2);

  int httpCode = http.POST(payload);
  String firstHeader = http.header("Set-Cookie");
  setCookie(gettingCookie(firstHeader));

  if (httpCode > 0)
  {
    Serial.print("\r\nlogin_check");
    Serial.println((httpCode));
  }

}

void DataStream:: two_auth_check()
{
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure(); 

  http.begin(client , "https://panel.rcponline.pl/two_auth_check");
  http.addHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36");
  http.addHeader("Cookie", "PHPSESSID=" + getCookie());
  http.addHeader("Referer" , "https://panel.rcponline.pl/login/");

  int httpCode = http.GET();

  if (httpCode > 0)
  {
    Serial.print("\r\ntwo_auth_check:   ");
    Serial.println((httpCode));
  }
}

void DataStream:: app()
{
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure(); 

  http.begin(client , "https://panel.rcponline.pl/app/");
  http.addHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36");
  http.addHeader("Cookie", "PHPSESSID=" + getCookie());
  http.addHeader("Referer" , "https://panel.rcponline.pl/login/");

  int httpCode = http.GET();

  if (httpCode > 0)
  {
    Serial.print("\n\rapp:   ");
    Serial.println(httpCode);
  }
}

void DataStream:: getMyStatus0()
{
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure(); 

  http.begin(client , "https://panel.rcponline.pl/dashboard/getMyStatus/0");
  http.addHeader("Cookie", "PHPSESSID=" + getCookie());
  http.addHeader("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36");
  http.addHeader("Referer" , "https://panel.rcponline.pl/app/");


  int httpCode = http.POST("");

  String status = "0";
    if (httpCode>0)
    {
      Serial.print("\n\rgetMyStatus0");
      Serial.println(httpCode);
      String Status_Code = http.getString();
      // String decodate_status = decodeUnicode(Status_Code);
      // Serial.println(decodate_status);
      Serial.print(extractPresenceStatus(Status_Code));
  
    } 
}   

String DataStream::decodeUnicode(String input)
{
  input.replace("\\u003C", "<");
  input.replace("\\u003E", ">");
  input.replace("\\u0022", "\"");
  input.replace("\\u002F", "/");
  input.replace("\\u00f3", "ó");
  input.replace("\\u015b", "ś");
  input.replace("\\u0142", "ł");
  input.replace("\\u017c", "ż");
  input.replace("\\u017a", "ź");
  input.replace("\\u0107", "ć");
  input.replace("\\u0105", "ą");
  input.replace("\\u0119", "ę");
  input.replace("\\u00f1", "ń");
  return input;
}

String DataStream :: extractPresenceStatus(String response) 
{
    response.replace("\\u003C", "<");
    response.replace("\\u003E", ">");
    response.replace("\\u0022", "\"");
    response.replace("\\u015b", "ś");
    response.replace("\\u0107", "ć");
    response.replace("\\u00f3", "ó");
    response.replace("\\u0142", "ł" );
    response.replace("\\u017c", "ż");

    const String start_marker = "<span class=\"fw-bolder fs-2\">";
    const String end_marker = "<\\/span>"; 

    int start_pos = response.indexOf(start_marker);
    if (start_pos == -1) 
    {
      return "Blad: Nie znaleziono poczatku statusu";
    }
    
    start_pos += start_marker.length();

    int end_pos = response.indexOf(end_marker, start_pos);
    if (end_pos == -1) 
    {
      return "Error"
    }

    String raw_status = response.substring(start_pos+20, end_pos-42);
    raw_status.trim();
    if (raw_status == "Na stanowisku")
    {
      setStatus("1");
    }
    else if( raw_status =="Na przerwie")
    {
      setStatus("2");
    }
    else if( raw_status == "Na wyjściu służbowym")
    {
      setStatus("3");
    }
    else if( raw_status == "Nie ma")
    {
      setStatus("4");
    }
    else
    {
      setStatus("0");
    }
    return raw_status;
}