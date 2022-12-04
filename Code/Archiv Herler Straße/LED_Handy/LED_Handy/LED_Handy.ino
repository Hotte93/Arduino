#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <EEPROM.h>





//----------------------------------------------------------------------------------------------------------------------
 
#define wifi_ssid "WGBums"
#define wifi_password "Kartoffelbrei1337"
#define mqtt_server "192.168.0.59"




  
//----------------------------------------------------------------------------------------------------------------------


WiFiClient espClient;  
PubSubClient client(espClient);  
  
bool status;  
int value[2];

//----------------------------------------------------------------------------------------------------------------------

 void setup() {  
  Serial.begin(9600);   
  EEPROM.begin(3);

  setup_wifi();  
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);  

  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  


  
  
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
      client.subscribe("esp4/LED_rot");
      client.subscribe("esp4/LED_blau");
      client.subscribe("esp4/LED_gruen");
      client.subscribe("esp4/LED_rot_gruen");
      client.subscribe("esp4/LED_rot_blau");
      client.subscribe("esp4/LED_gruen_blau");
      
      
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

      client.subscribe("esp4/LED_rot");
      client.subscribe("esp4/LED_blau");
      client.subscribe("esp4/LED_gruen");
      client.subscribe("esp4/LED_rot_gruen");
      client.subscribe("esp4/LED_rot_blau");
      client.subscribe("esp4/LED_gruen_blau");
      client.subscribe("esp4/LED_rot_gruen_blau");


     
if (strcmp(topic,"esp4/LED_rot")) {
      if(messageTemp =="esp4/LED_rot"){
      
      digitalWrite(D0, HIGH);
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      }
      
      }

    if (strcmp(topic,"esp4/LED_blau")) {
      if(messageTemp =="esp4/LED_blau"){
        
      digitalWrite(D0, LOW);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
    
      }
     
    }

    if (strcmp(topic,"esp4/LED_gruen")) {
      if(messageTemp =="esp4/LED_gruen"){

      digitalWrite(D0, LOW);
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);

      }
      }

    if (strcmp(topic,"esp4/LED_rot_gruen")) {
      if(messageTemp =="esp4/LED_rot_gruen"){

      digitalWrite(D0, HIGH);
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
      }
      } 

    if (strcmp(topic,"esp4/LED_rot_blau")) {
      if(messageTemp =="esp4/LED_rot_blau"){

      digitalWrite(D0, HIGH);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
      }
    }

    if (strcmp(topic,"esp4/LED_rot_blau_gruen")) {
       if(messageTemp =="esp4/LED_rot_blau_gruen"){

      digitalWrite(D0, HIGH);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, HIGH);
      }   
    }

    if (strcmp(topic,"esp4/aus")) {
      if(messageTemp =="esp4/aus"){
      

      digitalWrite(D0, LOW);
      digitalWrite(D1, LOW);
      digitalWrite(D2, LOW);
      } 
    }



  }



  