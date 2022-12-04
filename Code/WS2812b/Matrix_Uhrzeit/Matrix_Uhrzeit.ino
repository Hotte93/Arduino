//######################################################################################################################
/*
 *Beschreibung:
 *Programm zeigt die Uhrzeit auf einer 16x16 LED Matrix an.
 *Verbindung zum Server mittels MQTT
 *Neben derUrhzeit kann auch die Farbkombination der  Anzeige verändert werden.
*/
//######################################################################################################################

#include <ESP8266WiFi.h>
#include <string>
#include <iostream>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


//--------------------------------------------------------------

#define wifi_ssid "Vodafone-5E34"
#define wifi_password "fbNeGY4n6s7h42eK"
#define mqtt_server "192.168.0.59"

#define PIN        12 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 256 // Popular NeoPixel ring size

/*
        1  6  7  8
        2        9
        3 12 11 10
        4       13
        5 16 15 14  
*/  
#define Zahl1_1 61
#define Zahl1_2 60
#define Zahl1_3 59
#define Zahl1_4 58
#define Zahl1_5 57
#define Zahl1_6 66
#define Zahl1_7 93
#define Zahl1_8 98
#define Zahl1_9 99
#define Zahl1_10  100
#define Zahl1_11  91
#define Zahl1_12  68
#define Zahl1_13  101
#define Zahl1_14  102
#define Zahl1_15  89
#define Zahl1_16  70

#define Zahl2_1 157
#define Zahl2_2 156
#define Zahl2_3 155
#define Zahl2_4 154
#define Zahl2_5 153
#define Zahl2_6 162
#define Zahl2_7 189
#define Zahl2_8 194
#define Zahl2_9 195
#define Zahl2_10  196
#define Zahl2_11  187
#define Zahl2_12  164
#define Zahl2_13  197
#define Zahl2_14  198
#define Zahl2_15  185
#define Zahl2_16  166


#define Zahl3_1 54
#define Zahl3_2 53
#define Zahl3_3 52
#define Zahl3_4 51
#define Zahl3_5 50
#define Zahl3_6 73
#define Zahl3_7 86
#define Zahl3_8 105 
#define Zahl3_9 106
#define Zahl3_10  107
#define Zahl3_11  84
#define Zahl3_12  75
#define Zahl3_13  108
#define Zahl3_14  109
#define Zahl3_15  82
#define Zahl3_16  77

#define Zahl4_1 150
#define Zahl4_2 149    
#define Zahl4_3 148
#define Zahl4_4 147
#define Zahl4_5 146
#define Zahl4_6 169
#define Zahl4_7 182
#define Zahl4_8 201
#define Zahl4_9 202
#define Zahl4_10  203
#define Zahl4_11  180
#define Zahl4_12  171
#define Zahl4_13  204
#define Zahl4_14  205
#define Zahl4_15  178
#define Zahl4_16  173

//--------------------------------------------------------------
 


String messageTemp;
bool status;  
const int ResetPin = 4;
int ResetCounter =0;

int PIN_Red = 15;
int PIN_Green =15;
int PIN_Blue =0;
int Zahl =0;

int xred= 0;
int xgreen= 0;
int xblue= 0;
//--------------------------------------------------------------

struct FARBE
{
  int red;
  int blue;
  int green;
  
}Farbe;

//--------------------------------------------------------------


WiFiClient espClient;  
PubSubClient client(espClient); 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//######################################################################################################################

void setup() 
{

  Serial.begin(9600);   
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  setup_wifi();  
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);  
 

}

//######################################################################################################################
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
      WiFi.begin(wifi_ssid, wifi_password); 
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId= "ESP8266-";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      delay(100);
      if (client.connect(clientId.c_str()))  
      {
          Serial.println("connected");
          // Once connected, publish an announcement...
      
          // ... and resubscribe
          client.subscribe ("esp4/LED_Matrix_16x16");
          client.subscribe ("esp0/PIN_RED");
          client.subscribe ("esp0/PIN_GREEN");
          client.subscribe ("esp0/PIN_BLUE");
     
      } 
      else 
      {

          ResetCounter++;
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
 
          Serial.println(ResetCounter);

          if (ResetCounter >=5)
          {

              setup_wifi(); 
              client.setServer(mqtt_server, 1883); 
              client.setCallback(callback); 
              //digitalWrite(ResetPin, LOW);  
              //ResetCounter =0;
          }
     
          // Wait 5 seconds before retrying
          delay(5000);
      }
   }
}
//######################################################################################################################

void loop() 
{  
  if (!client.connected()) 
  {  
      reconnect();  
  }
  client.loop();

}  

//######################################################################################################################

void setup_wifi() 
{  
  delay(10);  
  // We start by connecting to a WiFi network  
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

//######################################################################################################################

void callback(char* topic, byte* message, unsigned int length) 
{

  Serial.print("Message arrived on topic: ");  
  Serial.print(topic);
  Serial.print(". Message: ");            


  String messageTempBuffer;   
  String messageTemp_red;
  String messageTemp_green;
  String messageTemp_blue;

                             
  for (int i = 0; i < length; i++) 
  {
      Serial.print((char)message[i]);
      messageTempBuffer += (char)message[i];
    
  }

  if((messageTempBuffer[0] == '0'  || messageTempBuffer[0] == '1' || messageTempBuffer[0] == '2' || messageTempBuffer[0] == '3'))
  {
      Serial.println("Bin drin");
      for(int i = 0; i < length; i++)
      {
          messageTemp += (char)message[i];
          Serial.print(messageTemp[i]);
      }
  }

  Serial.println();
     
  client.subscribe ("esp4/LED_Matrix_16x16");
  client.subscribe ("esp0/PIN_RED");
  client.subscribe ("esp0/PIN_GREEN");
  client.subscribe ("esp0/PIN_BLUE");
  client.subscribe ("esp0/LED_Aus");
 
  if( length >5 && length < 17)
  {
    
      int i=4;
   
      while(messageTempBuffer[i] != 'x')
      {
           
          messageTemp_red += messageTempBuffer[i];
          i++;
          yield();
      }
      i++;     
      while(messageTempBuffer[i] != 'y')
      {
            
          messageTemp_green += messageTempBuffer[i];
          i++;
          yield();
      }
      i++;
      while(messageTempBuffer[i] != 'z')
      {
            
          messageTemp_blue += messageTempBuffer[i];
          i++;
          yield();
      }
        
      PIN_Red = messageTemp_red.toInt();   
      PIN_Green =  messageTemp_green.toInt();
      PIN_Blue =  messageTemp_blue.toInt();    

      i=0;

      Serial.println(PIN_Red);
      Serial.println(PIN_Green);
      Serial.println(PIN_Blue);

  }

  if (messageTempBuffer == "off") 
  {

      for(int i=0; i<256; i++)
      {
          pixels.setPixelColor(i, pixels.Color(0,0,0));
      }
  
  }

  if (messageTemp[0] == '0'  || messageTemp[0] == '1' || messageTemp[0] == '2' || messageTemp[0] == '3')
  {

      pixels.clear(); // Set all pixel colors to 'off'
      for(int i=0; i<17; i++)
      { 
          pixels.setPixelColor(i, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
          pixels.setPixelColor((257-i), pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
          pixels.setPixelColor((i*16), pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
          pixels.setPixelColor(((i*16)+15), pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      }
  }


  if(messageTemp[0] == '0')
  {
      //Zahl1-0 
      pixels.setPixelColor(Zahl1_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
  }
     
  else if(messageTemp[0] == '1')
  {  
      //Zahl 1-1   
      pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
  } 

  else if(messageTemp[0] == '2')
  {  
      //Zahl 1-2
      pixels.setPixelColor(Zahl1_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));

   }
    
   else if(messageTemp[0] == '3')
   {     
      //Zahl 1-3
      pixels.setPixelColor(Zahl1_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
   }
   
   else if(messageTemp[0] == '4')
   {      
      //Zahl 1-4
      pixels.setPixelColor(Zahl1_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
   }
    
    
   else if(messageTemp[0] == '5')
   {    
      //Zahl 1-5
      pixels.setPixelColor(Zahl1_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));

   }
   
    
   else if(messageTemp[0] == '6')
   {     
      //Zahl 1-6 
      pixels.setPixelColor(Zahl1_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
   }
    

   else if(messageTemp[0] == '7')
   { 
      //Zahl 1-7
      pixels.setPixelColor(Zahl1_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
   }
   
    
   else if(messageTemp[0] == '8')
   {  
      //Zahl 1-8
      pixels.setPixelColor(Zahl1_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));;
      pixels.setPixelColor(Zahl1_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
      pixels.setPixelColor(Zahl1_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));

   }
   
    
   else if(messageTemp[0] == '9')
   {  
        //Zahl 1-9
        pixels.setPixelColor(Zahl1_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl1_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
   }
  
   
   if(messageTemp[1] == '0')
   {  
        //Zahl 2-0
        pixels.setPixelColor(Zahl2_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
   }
   
     
   else if(messageTemp[1] == '1')
   { 
        //Zahl 2-1
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        
   }
    
   
   else if(messageTemp[1] == '2')
   {
        //Zahl 2-2 
        pixels.setPixelColor(Zahl2_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
   }
    
    
   else if(messageTemp[1] == '3')
   { 
        //Zahl 2-3
        pixels.setPixelColor(Zahl2_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
   }
   
    
    else if(messageTemp[1] == '4')
    { 
        //Zahl 2-4
        pixels.setPixelColor(Zahl2_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[1] == '5')
    { 
        //Zahl 2-5
        pixels.setPixelColor(Zahl2_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[1] == '6')
    { 
        //Zahl 2-6
        pixels.setPixelColor(Zahl2_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
   
    else if(messageTemp[1] == '7')
    { 
        //Zahl 2-7
        pixels.setPixelColor(Zahl2_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[1] == '8')
    { 
        //Zahl 2-8
        pixels.setPixelColor(Zahl2_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));;
        pixels.setPixelColor(Zahl2_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[1] == '9')
    { 
        //Zahl 2-9
        pixels.setPixelColor(Zahl2_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl2_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
       
    if(messageTemp[2] == '0')
    {
        //Zahl 3-0 
        pixels.setPixelColor(Zahl3_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[2] == '1')
    {
        //Zahl 3-1 
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[2] == '2')
    { 
        //Zahl 3-2
        pixels.setPixelColor(Zahl3_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }

   
    
    else if(messageTemp[2] == '3')
    {
        //Zahl 3-3 
        pixels.setPixelColor(Zahl3_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[2] == '4')
    {
        //Zahl 3-4 
        pixels.setPixelColor(Zahl3_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
     else if(messageTemp[2] == '5')
     { 
        //Zahl 3-5
        pixels.setPixelColor(Zahl3_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
     }
    
    
     else if(messageTemp[2] == '6')
     {
        //Zahl 3-6 
        pixels.setPixelColor(Zahl3_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
     }
    

     else if(messageTemp[2] == '7')
     { 
        //Zahl 3-7
        pixels.setPixelColor(Zahl3_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
     }
     
    
     else if(messageTemp[2] == '8')
     { 
        //Zahl 3-8
        pixels.setPixelColor(Zahl3_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));;
        pixels.setPixelColor(Zahl3_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
     }
     
   
     else if(messageTemp[2] == '9')
     { 
        //Zahl 3-9
        pixels.setPixelColor(Zahl3_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl3_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
     }
     
    
     if(messageTemp[3] == '0')
     { 
        //Zahl 4-0 
        pixels.setPixelColor(Zahl4_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
     }
     
     
     else if(messageTemp[3] == '1')
     {
        //Zahl 4-1
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
     }
     
   
     else if(messageTemp[3] == '2')
     {
        //Zahl 4-2
        pixels.setPixelColor(Zahl4_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
     }
    
    
    else if(messageTemp[3] == '3')
    {
        //Zahl 4-3
        pixels.setPixelColor(Zahl4_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[3] == '4')
    {
        //Zahl 4-4
        pixels.setPixelColor(Zahl4_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
  
    else if(messageTemp[3] == '5')
    {
        //Zahl 4-5
        pixels.setPixelColor(Zahl4_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[3] == '6')
    {
        //Zahl 4-6
        pixels.setPixelColor(Zahl4_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[3] == '7')
    {
        //Zahl 4-7
        pixels.setPixelColor(Zahl4_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
    
    
    else if(messageTemp[3] == '8')
    {
        //Zahl 4-8
        pixels.setPixelColor(Zahl4_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_15, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));;
        pixels.setPixelColor(Zahl4_16, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_5, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_4, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }

    
    else if(messageTemp[3] == '9')
    {
        //Zahl 4-9
        pixels.setPixelColor(Zahl4_1, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_6, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_7, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_8, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_9, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_10, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_13, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_14, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_11, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_12, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_3, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
        pixels.setPixelColor(Zahl4_2, pixels.Color(PIN_Red, PIN_Green, PIN_Blue));
    }
  

  pixels.show();

}
