#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//----------------------------------------------------------------------------------------------------------------------


#define wifi_ssid "Vodafone-5E34"
#define wifi_password "fbNeGY4n6s7h42eK"
#define mqtt_server "192.168.0.59"

#define PIN        12 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 103 // Popular NeoPixel ring size
#define Relais     14

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels

//----------------------------------------------------------------------------------------------------------------------


WiFiClient espClient;  
PubSubClient client(espClient);  
  
bool status;  
//const int OutputPin2 =  12;  
const int ResetPin = 4;
int ResetCounter =0;


//----------------------------------------------------------------------------------------------------------------------

 void setup() {  
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  
  Serial.begin(9600);   
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  setup_wifi();  
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);  
  //pinMode(PIN, OUTPUT);
  pinMode(Relais, OUTPUT);
  //pinMode(ResetPin, OUTPUT);
  //digitalWrite(OutputPin2, HIGH);
  digitalWrite(Relais, HIGH);
 }  
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
      
      client.subscribe("esp2/LED_Fernseh_Regal");
      
    } else {

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

 
 client.subscribe ("esp2/LED_Fernseh_Regal");


 if (strcmp(topic,"esp2/LED_Fernseh_Regal")) {
    Serial.print("Changing output to ");
    if(messageTemp == "on LED_Fernseh_Regal"){

   pixels.clear();
    
       int j=0;
       for(int i=52; i<NUMPIXELS; i++) { // For each pixel...
       j++;
    

    pixels.setPixelColor(i, pixels.Color(126, 45, 15));
    pixels.setPixelColor((52-j), pixels.Color(126, 45, 15));
    delay(50);

    pixels.show();   // Send the updated pixel colors to the hardware.
       }
      
      delay(200);
      //pixels.clear();
    }
    else if(messageTemp == "off LED"){

   
    int j=0;
       for(int i=0; i<54; i++) { // For each pixel...
       //j++;
    

    
    pixels.setPixelColor((103-i), pixels.Color(0, 0, 0));
    pixels.setPixelColor((i), pixels.Color(0, 0, 0));
    delay(50);

    pixels.show();   // Send the updated pixel colors to the hardware.
    
    
    /*  digitalWrite(Relais, LOW);
     Serial.println("Test");
      //digitalWrite(PIN, LOW);
      delay(200);
      digitalWrite(Relais, HIGH);
      */
    }
  pixels.setPixelColor(53, pixels.Color(0, 0, 0));
  delay(50);

  }
  
 }
  
  }
