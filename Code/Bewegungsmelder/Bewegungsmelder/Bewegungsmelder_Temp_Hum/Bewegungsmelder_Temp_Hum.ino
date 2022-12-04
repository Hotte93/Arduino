#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"
#include "Adafruit_Sensor.h"
#include "SPI.h"

//----------------------------------------------------------------------------------------------------------------------

#define wifi_ssid "Vodafone-5E34"
#define wifi_password "fbNeGY4n6s7h42eK"
#define mqtt_server "192.168.0.59"

#define motion_topic "esp5/motion"
#define temperature_topic "esp1/temp"
#define humidity_topic "esp1/hum"



//----------------------------------------------------------------------------------------------------------------------

#define DHTPIN 4         
#define DHTTYPE DHT22  

DHT dht(DHTPIN, DHTTYPE); //dht Pin und Typ übergeben  


WiFiClient espClient;  
PubSubClient client(espClient);  


bool status;  
//const int OutputPin2 =  12;  
const int ResetPin = 5;
int ResetCounter = 0;

int bewegung= 12;
int bewegungsstatus=0;

 


 void setup() {  

  
  pinMode(bewegung, INPUT);
  pinMode(ResetPin, OUTPUT);
  digitalWrite(ResetPin, HIGH);


  Serial.begin(9600); 
  dht.begin();  
  setup_wifi();  
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);  

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


  

    client.subscribe("esp1/req");

  
    float h = dht.readHumidity();   
    float t = dht.readTemperature();

    static char Temperatur[7];
    static char Luftfeuchte[7];

    Serial.println(h);
    Serial.println(t);

    dtostrf(h, 6, 2, Temperatur);
    dtostrf(t, 6, 2, Luftfeuchte);  

  
    client.publish(temperature_topic, Temperatur);
    client.publish(humidity_topic, Luftfeuchte);

   ResetCounter ++;

  delay(100);

  Serial.println(ResetCounter);

  if(ResetCounter ==1)
  {
    digitalWrite(ResetPin, LOW);
    delay(50);
    digitalWrite(ResetPin, HIGH);

    ResetCounter =0;
  }



 // Serial.println();

  }

//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
     WiFi.begin(wifi_ssid, wifi_password); 
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId= "ESP8266-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    delay(100);
    if (client.connect(clientId.c_str()))  {
      Serial.println("connected");
      // Once connected, publish an announcement...
      
      // ... and resubscribe
        client.subscribe("esp1/req");
      
    } else {

        
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
 
        //Serial.println(ResetCounter);

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

  

    bewegungsstatus=digitalRead(bewegung);



    if (bewegungsstatus == 1)
    {

      //client.publish(motion_topic, "1");

      //delay(180000);

      //client.publish(motion_topic, "0");

    }
    
    delay(50);

      //static char Motion[0];
      //dtostrf(bewegungsstatus, 6, 2, Motion);

   

   


 }  
//----------------------------------------------------------------------------------------------------------------------   

 //----------------------------------------------------------------------------------------------------------------------
