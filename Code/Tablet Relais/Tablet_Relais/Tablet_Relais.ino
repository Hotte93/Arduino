#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>



//----------------------------------------------------------------------------------------------------------------------


#define wifi_ssid "Vodafone-5E34"
#define wifi_password "fbNeGY4n6s7h42eK"
#define mqtt_server "192.168.0.59"




//----------------------------------------------------------------------------------------------------------------------
 
const int RelaisPin = 12;


//----------------------------------------------------------------------------------------------------------------------

WiFiClient espClient;  
PubSubClient client(espClient);


//#######################################################################################################################

void setup() 
{  

    pinMode(RelaisPin, OUTPUT);
    digitalWrite(RelaisPin, HIGH);
    Serial.begin(9600);
    setup_wifi();  
    client.setServer(mqtt_server, 1883); 
    client.setCallback(callback);
    client.connect("ddd123");
    client.subscribe("esp6/relais");
    
     

}  
 

//#######################################################################################################################


void setup_wifi() 
{  
  
  delay(10);  
  Serial.println();  
  Serial.print("Connecting to ");  
  Serial.println(wifi_ssid);  
  WiFi.begin(wifi_ssid, wifi_password); 
   
  while (WiFi.status() != WL_CONNECTED) 
  {  
    Serial.print("."); 
    delay(500);  
  }
    
  Serial.println("");  
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");  
  Serial.println(WiFi.localIP());  
  
}  

//#######################################################################################################################


void callback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

 
  for (int i = 0; i < length; i++) 
  {
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
  }


  client.subscribe("esp6/relais");
  delay(3000);
  
  digitalWrite(RelaisPin, LOW);
  delay(2000);
  digitalWrite(RelaisPin, HIGH);
  delay(2000);
  
  digitalWrite(RelaisPin, LOW);
  delay(2000);
  digitalWrite(RelaisPin, HIGH);
  delay(2000);
  
  digitalWrite(RelaisPin, LOW);
  delay(2000);
  digitalWrite(RelaisPin, HIGH);
  delay(2000);


  
 
}


//#######################################################################################################################

   
void loop() 
{  

  if((WiFi.status() != WL_CONNECTED))
  {
      setup_wifi(); 
  }

  client.connect("ddd123");
  client.subscribe("esp6/relais");
  client.loop();
  
}  
//#######################################################################################################################
