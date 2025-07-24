//Libraries
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

//Headers Files
#include "SerialMenu.h"
#include "MyWiFi.h"
#include "DataStream.h"




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
  delay(1000);                                  
  login_check();
  delay(1000);                                    
  two_auth_check();
  delay(1000);                                    
  app();                              // without this request you can have problems Code Status of getMyStatus0 is often 500 so you should use it

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

  //bool stateOfHeader = http.hasHeader("Set-Cookie");          // very useful while debuging stateOfHeader = 0 if "Set-Cookie" header is not obtained, and stateOfHeader = 1 if is obtained
  //Serial.println(stateOfHeader);                              //
  String csrf = "0";
    if (httpCode>0)
    {
      Serial.print("\n\rlogin :");
      Serial.println(httpCode);
      if (httpCode == HTTP_CODE_OK)
      {                                                                         //
        int len = http.getSize();                                               // because http.getString() is to large to be saved in ESP8266 memory you have to make smaller buffors to handle it 
                                                                                //
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
  String employeeStatus = getStatus();
//  Serial.println(employeeStatus);           //Usefull while debugging  
  if (employeeStatus == "1" )
  {
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);
  }
  else if (employeeStatus == "2")
  {
    digitalWrite(12,LOW);
    digitalWrite(13,HIGH);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);
  }
  else if (employeeStatus == "3" )
  {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    digitalWrite(14,HIGH);
    digitalWrite(15,LOW);
  }
  else if (employeeStatus == "4" )
  {
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    digitalWrite(14,LOW);
    digitalWrite(15,HIGH);    
  }

}

String DataStream:: gettingCsrf(uint8_t* b)
{
  String text = String((char*)b);
  const String tagCsrf = "<input type=\"hidden\" name=\"_csrf_token\" id=\"_csrf_token\" " ;    /// im not proud of it 

  String foundedCsrf = "";

  int startTagPosition = text.indexOf(tagCsrf);

  if (startTagPosition != -1)
  {
    int startQuotationMark = text.indexOf('value="',startTagPosition);
    int endQuotationMark = text.indexOf('" />',startQuotationMark +1);
    if (startQuotationMark != -1 && endQuotationMark !=-1)
    {
      String csrfText = text.substring(64 + startQuotationMark, endQuotationMark -3);
      return csrfText;
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
  
  const String tagCookie = "PHPSESSID=";
  const String tagEndCookie = "; expires";
  
  String foundedCookie = "";

  int startTagPosition = tekst.indexOf(tagCookie);

  if (startTagPosition != -1) 
  {
    int startQuotationMark = tekst.indexOf(tagCookie, startTagPosition);
    int endQuotationMark = tekst.indexOf(tagEndCookie, startQuotationMark + 1);

    if (startQuotationMark != -1 && endQuotationMark != -1) 
    {
      String cookieText = tekst.substring(startTagPosition+10,endQuotationMark);
      return cookieText;
    }
  }
  if (foundedCookie.length() > 0) {
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
    Serial.print("\r\nlogin_check Status Code : ");
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
    Serial.print("\r\ntwo_auth_check Status Code:   ");
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
    Serial.print("\n\rapp Status Code:   ");
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
      Serial.print("\n\rgetMyStatus0 Status Code:   ");
      Serial.println(httpCode);
      Serial.println("");
      String pressenceStatus = http.getString();
      Serial.println(extractPresenceStatus(pressenceStatus));
      
  
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

// Here you can add more Unicode signs if you use other language that Polish
    response.replace("\\u003C", "<");
    response.replace("\\u003E", ">");
    response.replace("\\u0022", "\"");
    response.replace("\\u015b", "ś");
    response.replace("\\u0107", "ć");
    response.replace("\\u00f3", "ó");
    response.replace("\\u0142", "ł" );
    response.replace("\\u017c", "ż");
//

    const String tagStatus = "<span class=\"fw-bolder fs-2\">";
    const String tagEndStatus = "<\\/span>"; 

    int startPosition = response.indexOf(tagStatus);
    if (startPosition == -1) 
    {
      return "Blad: Nie znaleziono poczatku statusu";
    }
    
    startPosition += tagStatus.length();

    int endPosition = response.indexOf(tagEndStatus, startPosition);
    if (endPosition == -1) 
    {
      return "Error";
    }

    String rawStatus = response.substring(startPosition+20, endPosition-42);
    rawStatus.trim();
    if (rawStatus == "Na stanowisku")
    {
      setStatus("1");
    }
    else if( rawStatus =="Na przerwie")
    {
      setStatus("2");
    }
    else if( rawStatus == "Na wyjściu służbowym")
    {
      setStatus("3");
    }
    else if( rawStatus == "Nie ma")
    {
      setStatus("4");
    }
    else
    {
      setStatus("0");
    }
    return rawStatus;
}