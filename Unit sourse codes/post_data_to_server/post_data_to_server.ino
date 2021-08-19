#define RETRY_LIMIT  20
#include <WiFi.h>
//#include <ESP8266WiFi.h>
#include <HTTPClient.h>
//#include <ESP8266HTTPClient.h>

const char* ssid = "Fed Geek";
const char* password = "11002299";

void setup()
{
 
    Serial.begin(9600);
    WiFi.begin(ssid,password);
    while (WiFi.status()!= WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP Address");
    Serial.println(WiFi.localIP());
 
}

void loop()
{
    int rtl = RETRY_LIMIT;
    float h =  22.22;
    float t= 11.11;
    delay(500);
    while (isnan(t) || isnan(h)){
      Serial.println("Check sensor again - " + rtl);
      h =  33.33;
      t= 44.44;
      delay(500);
      if (--rtl <1){
        ESP.restart(); // A Restart fixed this.
      }
    }
    
    //Open a connection to the server
      HTTPClient http;
      http.begin("http://192.168.8.195/water_flow_in.php");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    //POST request.
      //int httpResponseCode = http.POST("data1=" + String(t) +"&data2=" + String(h));
      String md1 = "121";
      String md2 = "20";
      String md3 = "my data3.time";
      
      int httpResponseCode = http.POST("data1=" + md1 +"&data2=" + md2+ "&data3=" + md3);

      if (httpResponseCode >0){    //check for a return code
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      }
      else{
        Serial.print("Error on sending post");
        Serial.println(httpResponseCode);
      }
      http.end(); //closde the HTTP request.

    //Monitor values
      Serial.println("Temp = " + String(t));
      Serial.println("humidity = " + String(h));
    
    
   //wait 2 minutes for next reading
    delay(3000);
    
    
}
