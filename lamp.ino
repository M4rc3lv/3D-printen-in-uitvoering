#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

#define WIFI_SSID "Wifi SSID"
#define WIFI_PASSWORD "Wifi Wachtwoord"

#define PIN         D8
#define LED_COUNT   24

Adafruit_NeoPixel LED = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ400);

const char *Printers[]={
 "MK3.9", "http://192.168.0.10", "5B9379B9CSecret4BB981B1C728", // My MK4 Upgrade printer
 "MK4","http://192.168.0.49:80","hdp4DXSecretpN", // My first MK4 printer
 "MK4-2","http://192.168.0.106:80","MSecretUnozg2y" // My Prusa Mini printer
};

ESP8266WiFiMulti D1Mini;
#define NUMPRINTERS (sizeof(Printers) / sizeof(Printers[0])/3)

void setup() {
 Serial.begin(115200);
 LED.begin();
 Blauw();
 WiFi.mode(WIFI_STA);
 D1Mini.addAP(WIFI_SSID, WIFI_PASSWORD);
 Uit(); 
}

void loop() {  
 //Aan();
 //while(1) delay(5000);
 bool Printing=false;
 if(D1Mini.run()== WL_CONNECTED){    
  WiFiClient Wifi;
  HTTPClient http;
  for(int i=0; i<NUMPRINTERS; i++) {
   Serial.printf("Checking printer %d %s...\r\n",i,Printers[i*3]);
   http.begin(Wifi,String(Printers[i*3+1])+String("/api/job"));
   http.addHeader("X-Api-Key", Printers[i*3+2]);        
   int httpCode = http.GET();
   if(httpCode > 0) {     
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    String CurStatus = doc["state"].as<String>();
    if(CurStatus.indexOf("Printing")>=0) {
     Serial.printf("Printer %d %s is aan het printen\r\n",i,Printers[i*3]);
     Printing=true;
     Rood();
    }
   }
  }
 }
 if(!Printing) Uit();

 long t=20000;
 while(t>0) {
  if(Printing) {
   Rood();
   delay(500);
   Uit();
   delay(400);
   t-=900;
  }
  else
   {delay(1000); t-=1000;}
 } 
 if(Printing) Rood();
}


void Aan() {
 for(int i=0; i<LED_COUNT; i++) {
  LED.setPixelColor(i,LED.Color(160,94,0));  
 } 
 LED.show();
}

void Rood() {
 for(int i=0; i<LED_COUNT; i++) {
  LED.setPixelColor(i,LED.Color(100,0,0));  
 } 
 LED.show();
}


void Blauw() {
 for(int i=0; i<LED_COUNT; i++) {
  LED.setPixelColor(i,LED.Color(0,0,64));  
 } 
 LED.show();
}


void Uit() {
 for(int i=0; i<LED_COUNT; i++) {
  LED.setPixelColor(i,LED.Color(0,0,0));  
 } 
 LED.show();
}
