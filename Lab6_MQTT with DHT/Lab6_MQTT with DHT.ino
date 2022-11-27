//ok
#include "DHT.h"
#include <WiFi.h>  
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// DHT22 Declaration

#define DHTPIN 4
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

// Motor Pin Declaration
#define MotorPin 2

//---- WiFi settings
const char* ssid = "Shiru";
const char* password = "12345678";




//---- MQTT Broker settings
const char* mqtt_server = "192.168.137.247"; // replace with your broker url

const int mqtt_port =1883;


WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];


//Mosquitto Publishing channels
const char* sensor1_topic= "ESPone/1"; // publishing temperature
const char*  sensor2_topic="ESPone/2"; // publishing humidity
//const char*  sensor3_topic="Motor/1"; // Start
//const char*  sensor4_topic = "Motor/2" // Stop

//Mosquitto Subscription channels

const char* command1_topic="ESPone/#";  // subscribing
const char* command2_topic="Motor/#";   // ESPone subscribing to rpi to get info from python application


//==========================================
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //=======
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //=======
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}


//=====================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);  //you could make this static
    // Attempt to connect
    if (client.connect(clientId.c_str())){//, mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(command1_topic);   // subscribe the topics here
      client.subscribe(command2_topic);   // subscribe the topics here
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(1500);
    }
  }
}

//================================================
void setup() {
  Serial.begin(115200);
  dht.begin();
  //while (!Serial) delay(1);
  Serial.println("Setting up");
  setup_wifi();
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(MotorPin, OUTPUT); 
//
//  #ifdef ESP8266
//    espClient.setInsecure();
//  #else   // for the ESP32
//    espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
//  #endif
  
  client.setServer(mqtt_server, 1883 );//mqtt_port
  client.setCallback(callback);
}


//================================================
void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  // Reading Temperature and Humidity Values

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");


  // MQTT can only transmit strings
  String hs="Hum: "+String((float)h)+" % ";
  String ts="Temp: "+String((float)t)+" C ";

  // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
  if (client.publish(sensor1_topic, String(t).c_str())) {
    Serial.println("Temperature sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Temperature failed to send. Reconnecting to MQTT Broker and trying again");
    //client.connect(clientID, mqtt_username, mqtt_password);
   // delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    publishMessage(sensor1_topic, String(t),true);
  }

  // PUBLISH to the MQTT Broker (topic = sensor2_topic=Humidity, defined at the beginning)
  if (client.publish(sensor2_topic, String(h).c_str())) {
    Serial.println("Humidity sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
    //client.connect(clientID, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    publishMessage(sensor2_topic, String(h),true);
    Serial.println("Why is this here! @ Humidity section");
  }

    //client.disconnect();  // disconnect from the MQTT broker
    delay(3000);       // print new values every 1 Minute

    
  }

//=======================================  
// This void is called every time we have a message from the broker

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
  
  // --- check the incomming message
    if( strcmp(topic,command1_topic) == 0){
     if (incommingMessage.equals("1")) digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on 
     else digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off 
  }

   // --- check for other commands
   
   //Motor Control function goes here
    if( strcmp(topic,command2_topic) == 0){
     if (incommingMessage.equals("1")) {

        digitalWrite(MotorPin, HIGH);
     }

     else if (incommingMessage.equals("0")){

        digitalWrite(MotorPin, LOW);
       
      } 
  }
 
}


//======================================= publising as string
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message published ["+String(topic)+"]: "+payload);
}