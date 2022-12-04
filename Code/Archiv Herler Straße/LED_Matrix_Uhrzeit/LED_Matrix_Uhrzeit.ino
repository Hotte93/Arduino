#include <ESP8266WiFi.h> 
#include <Adafruit_NeoPixel.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define PIN D2 // Pin where NeoPixels are connected

const char *ssid     = "WGBums";
const char *password = "Kartoffelbrei1337";

//Zeitverschiebung UTC <-> MEZ (Winterzeit) = 3600 Sekunden (1 Stunde)
//Zeitverschiebung UTC <-> MEZ (Sommerzeit) = 7200 Sekunden (2 Stunden)
Adafruit_NeoPixel strip(256, PIN, NEO_GRB + NEO_KHZ800);

const long utcOffsetInSeconds = 3600;


char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Ich verbinde mich mit dem Internet...");
  }
  
  Serial.println("Ich bin mit dem Internet verbunden!");

  timeClient.begin();

  pinMode(PIN, OUTPUT);

}

void loop() {
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.println(timeClient.getHours());

  if (timeClient.getHours() == 21){

    Serial.println("Test");
  }
  Serial.println(timeClient.getMinutes());
  Serial.println(timeClient.getSeconds());

  delay(3000);
  
}