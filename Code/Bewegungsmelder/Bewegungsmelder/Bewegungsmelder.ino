#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

//----------------------------------------------------------------------------------------------------------------------

#define wifi_ssid "Vodafone-5E34"
#define wifi_password "fbNeGY4n6s7h42eK"
#define mqtt_server "192.168.0.59"
#define motion_topic "esp5/motion"


//----------------------------------------------------------------------------------------------------------------------


WiFiClient espClient;  
PubSubClient client(espClient);  

//----------------------------------------------------------------------------------------------------------------------
  
bool status;  
const int ResetPin = 14;
int ResetCounter =0;

int bewegung= 12;
int bewegungsstatus=0;



//#######################################################################################################################

void setup() 
{  

  pinMode(ResetPin, OUTPUT);
  digitalWrite(ResetPin, HIGH);
  Serial.begin(9600);   
  setup_wifi();  
  client.setServer(mqtt_server, 1883); 
  client.connect("asd123");
  client.setCallback(callback);  

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
 
}
  
//#######################################################################################################################

   
void loop() 
{  
 
   bewegungsstatus=digitalRead(bewegung);

   Serial.println(bewegungsstatus);
   
   if((WiFi.status() != WL_CONNECTED))
      {
          setup_wifi(); 
      }

   if (bewegungsstatus == 1)
   {

    
      client.connect("asd123");
      client.publish(motion_topic, "1");
         
   }
    
   delay(100);

   ResetCounter++;
  
   if(ResetCounter > 1200)
   {
      ResetCounter =0;
      digitalWrite(ResetPin, LOW);
   }
    

}  
//#######################################################################################################################
