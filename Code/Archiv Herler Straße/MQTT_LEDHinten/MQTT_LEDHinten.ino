#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"





//----------------------------------------------------------------------------------------------------------------------
#define DHTPIN 4         
#define DHTTYPE DHT22  

#define wifi_ssid "WGBums"
#define wifi_password "Kartoffelbrei1337"
#define mqtt_server "192.168.0.60"

#define temperature_topic "esp1/temp"
#define humidity_topic "esp1/hum"

//----------------------------------------------------------------------------------------------------------------------

DHT dht(DHTPIN, DHTTYPE); //dht Pin und Typ übergeben  
WiFiClient espClient;  
PubSubClient client(espClient);  
  
bool status;  
const int OutputPin2 =  12;  


//----------------------------------------------------------------------------------------------------------------------

 void setup() {  
  Serial.begin(9600);   
  dht.begin();
  setup_wifi();  
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);  
  pinMode(OutputPin2, OUTPUT);
  digitalWrite(OutputPin2, HIGH);
 }  
//----------------------------------------------------------------------------------------------------------------------
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId= "ESP8266-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))  {
      Serial.println("connected");
      // Once connected, publish an announcement...
      
      // ... and resubscribe
      
      client.subscribe("esp3/LED");
      
    } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
   }
}
//----------------------------------------------------------------------------------------------------------------------
   
 void loop() {  
  if (!client.connected()) {  
    reconnect();  
  }
  client.loop();

  float h = dht.readHumidity();   
  float t = dht.readTemperature();

  static char Temperatur[7];
  static char Luftfeuchte[7];

  dtostrf(h, 6, 2, Temperatur);
  dtostrf(t, 6, 2, Luftfeuchte);  



  client.publish(temperature_topic, String(t).c_str());
  client.publish(humidity_topic, String(h).c_str());
    
  Serial.println(h);
  Serial.print(t);
  delay(500);
  
 }  
//----------------------------------------------------------------------------------------------------------------------   
 void setup_wifi() {  
  delay(10);  
  // We start by connecting to a WiFi network  
  Serial.println();  
  Serial.print("Connecting to ");  
  Serial.println(wifi_ssid);  
  WiFi.begin(wifi_ssid, wifi_password);  
  while (WiFi.status() != WL_CONNECTED) {  
   Serial.print("."); 
   delay(500);  
  }  
  Serial.println("");  
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");  
  Serial.println(WiFi.localIP());  
 }  
 //----------------------------------------------------------------------------------------------------------------------
  void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

 
 client.subscribe ("esp3/LED");


 if (strcmp(topic,"esp3/LED")) {
    Serial.print("Changing output to ");
    if(messageTemp == "on LED"){
    
      Serial.println("on LED");
      digitalWrite(OutputPin2, LOW);   //Invertiertes Signal
      
      delay(200);
    }
    else if(messageTemp == "off LED"){
    
      Serial.println("off LED");
      digitalWrite(OutputPin2, HIGH);  
      
      delay(200);
    }
  

  }
  

  }
  
