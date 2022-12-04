#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


#define wifi_ssid "WGBums"
#define wifi_password "Kartoffelbrei1337"

#define mqtt_server "192.168.0.59"
  



   
WiFiClient espClient;  
PubSubClient client(espClient);  
  
bool status;  
const int OutputPin = 5;  // der Output Pin wo das Relais angehängt ist
const int OutputPin2 =  12;  
 void setup() {  
  Serial.begin(9600);   
  setup_wifi();  
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);  
  pinMode(OutputPin, OUTPUT);
  digitalWrite(OutputPin, HIGH);
  pinMode(OutputPin2, OUTPUT);
  digitalWrite(OutputPin2, HIGH);
 }  

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
      client.subscribe("esp3/Heiz");
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
   
 void loop() {  
  if (!client.connected()) {  
    reconnect();  
  }
  client.loop();
 }  
   
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

 client.subscribe ("esp3/Heiz");
 client.subscribe ("esp3/LED");

  if (strcmp(topic,"esp3/Heiz")) {
    Serial.print("Changing output to ");
    if(messageTemp == "on Heiz"){
    
      Serial.println("on Heiz");
      digitalWrite(OutputPin, LOW);   //Invertiertes Signal
      
      delay(200);
    }
    else if(messageTemp == "off Heiz"){
    
      Serial.println("off");
      digitalWrite(OutputPin, HIGH);  
      
      delay(200);
    }
  

  }

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
  
  
