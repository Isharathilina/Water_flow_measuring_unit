#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include "FS.h"
#include "SPIFFS.h"


// wifi data
const char *ssid     = "Fed Geek";
const char *password = "11002299";
String serverDist = "http://192.168.8.195/water_flow_in.php";

const long utcOffsetInSeconds = 19800;  // calibarate for srilanka
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define FORMAT_SPIFFS_IF_FAILED false //true
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define RETRY_LIMIT  20 // for sendig data

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// flow meeter
volatile int  Pulse_Count;
volatile int  Pulse_Count2;
unsigned int  Liter_per_min;
unsigned int  TotalLiters;
unsigned long Current_Time, Loop_Time;



void setup()
{ 
   Serial.begin(9600); 
   attachInterrupt(digitalPinToInterrupt(23), Detect_Rising_Edge, RISING);
                                     
   Current_Time = millis();
   Loop_Time = Current_Time;
   
   if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }

   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
   } 

   WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(11, 0);
  display.println(F("~Welcome~"));
  display.display();      // Show initial text
  delay(2000);

  display.startscrollright(0x00, 0x0F);
  delay(1000);
  display.stopscroll();
  display.startscrollleft(0x00, 0x0F);
  delay(1000);
  display.stopscroll();
  
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(5, 25);
  display.println(F("Water Flow Measuring"));
  display.display();  
  delay(2000); 

  
} 


int delayCount=0;
String sdWriteData;
void loop ()    
{
   Current_Time = millis();
   timeClient.update();

   char displayData[6];
   char displayData2[6];
   char displayData3[10];
   String timeNow;
   itoa(Liter_per_min,displayData,10);
   itoa(TotalLiters,displayData2,10);
   timeNow = String(timeClient.getHours())+":"+String(timeClient.getMinutes())+":"+String(timeClient.getSeconds());
   timeNow.toCharArray(displayData3, 10);



   if(Current_Time >= (Loop_Time + 1000))
   {
      delayCount = delayCount+1;
      Loop_Time = Current_Time;
      Liter_per_min = (Pulse_Count / 7.5);
      TotalLiters = (Pulse_Count2 / (7.5*60));
      Pulse_Count = 0;
      Serial.print(Liter_per_min, DEC);
      Serial.println(" Liter/min");
      Serial.print(TotalLiters, DEC);
      Serial.println(" Total Liters");

      Serial.print(daysOfTheWeek[timeClient.getDay()]);
      Serial.print(", ");
      Serial.print(timeClient.getHours());
      Serial.print(":");
      Serial.print(timeClient.getMinutes());
      Serial.print(":");
      Serial.println(timeClient.getSeconds());


      // send data to server
      HTTPClient http;
      http.begin(serverDist);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    //POST request.
      //int httpResponseCode = http.POST("data1=" + String(t) +"&data2=" + String(h));
      String md1 = String(Liter_per_min);
      String md2 = String(TotalLiters);
      String md3 = timeNow;

      sdWriteData =md1+" L/min "+md2+" L "+md3+"\n"; // for sd write
    
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
        
   }

  

   
   display.clearDisplay();

   display.setTextSize(1); // Draw 2X-scale text
   display.setTextColor(WHITE);
   display.setCursor(0, 0);
   display.print(F("Flow Rate :- "));
   display.print(F(displayData));
   display.println(F(" L/min"));

   display.setCursor(0, 15);
   display.print(F("Total :- "));
   display.print(F(displayData2));
   display.println(F(" L"));

   display.setCursor(0, 30);
   display.print(F("Time :- "));
   display.print(F(displayData3));
   display.println(F(""));

   display.display();     

   

  if(delayCount==10){
    char sdWrite[30];
    sdWriteData.toCharArray(sdWrite, 30);
    
     //listDir(SPIFFS, "/", 0);
     //appendFile(SPIFFS, "/waterFlow.txt", sdWrite);
    // readFile(SPIFFS, "/waterFlow.txt");


     /*
    //writeFile(SPIFFS, "/waterFlow.txt", "Hello ");
    //renameFile(SPIFFS, "/hello.txt", "/foo.txt");
    //deleteFile(SPIFFS, "/waterFlow.txt");
    //testFileIO(SPIFFS, "/waterFlow.txt");
    //deleteFile(SPIFFS, "/waterFlow.txt");

    */

    delayCount =0;
    }
      

}
// end loop

void Detect_Rising_Edge()
{ 
   Pulse_Count++;
   Pulse_Count2++;
} 


// sd card / flash functions

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- frite failed");
    }
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
}
