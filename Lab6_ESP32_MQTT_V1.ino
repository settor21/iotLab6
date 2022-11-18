#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include <WifiClientSecure.h>



#define USE_SERIAL Serial
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */  
#define TIME_TO_SLEEP  15        /* Time ESP32 will go to sleep (in seconds) */      
RTC_DATA_ATTR int bootCount = 0;   //needed 


Servo myservo;
#define TRIG_PIN 33 // ESP32 pin GIOP33 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 34 // ESP32 pin GIOP34 connected to Ultrasonic Sensor's ECHO pin
#define LED_PIN  18 // LED pin set on/off based on water level
#define SDA_PIN 21 // SDA pin for LCD
#define SCL_PIN 22 // SCL pin for LCD
#define MOTOR_PIN 13 // Motor pin set on/off based on water level
#define MAX_DIST 60 //centimeters
#define LDR_PIN 35 // Light intensity
#define LDR_IND 32 // LED indicator

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
float duration_us = 0;
float distance_cm = 0;
float Water_level = 0;
int LEDR = LOW;

// ---- WiFi Settings
const char* ssid = "Shiru";
const char* password = "1345678";
int manualState  = LOW;
//int counter = 0;

// ---- MQTT Broker Settings
const char* mqtt_server = "192.168.137.202"; // replace with your broker url
const char* mqtt_username = "Shiru";
const char* mqtt_password = "babybaby";
const int mqtt_port =8884;

WifiClientSecure espClient;

PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

const char* sensor1_topic = "ESPone/1"; // publish
const char* sensor12_topic = "ESPone/2"; // publish
// const char* sensor2_topic =  "ESPTwo";

const char* command1_topic = "command1"; // subscribe

Servo servo;

WebServer server(80);

static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";




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

void LEDRfunct()
{ if (manualState == HIGH) {
    LEDR = !LEDR;
    digitalWrite(LED_PIN, LEDR);
    //counter++;
    String str = "OFF";    //very little returned
    if (LEDR == HIGH ) str = "ON";
    server.send(200, "text/plain", str);
    Serial.println("red");
  }
}

void manualfunct() {
  if (manualState == HIGH) {
    manualState = LOW;
  }
  else {
    manualState = HIGH;
  }
}


// ------------------------------------------------------------

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);  //you could make this static
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(command1_topic);   // subscribe the topics here
      //client.subscribe(command2_topic);   // subscribe the topics here
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// -----------------------------------------------

void setup() {
  // put your setup code here, to run once:

  // begin serial port
  Serial.begin (115200);

  Serial.println("Setting up");
  setup_wifi();

  #ifdef ESP8266
    espClient.setInsecure();
  #else   // for the ESP32
    espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
  #endif
  
  // initialize the lcd
  lcd.init();
  lcd.clear();
  lcd.backlight();

  // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);
  // configure the echo pin to input mode
  pinMode(LED_PIN, OUTPUT);

  pinMode(LDR_IND, OUTPUT);
  servo.attach(MOTOR_PIN);

  servo.write(0);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);


}

// -------------------------------------------------------------------------

void loop() {
  // put your main code here, to run repeatedly:
  delay(2);

  if (!client.connected()) reconnect();
  client.loop();

  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  // calculate the distance
  distance_cm = 0.017 * duration_us;
  Water_level = distance_cm;
  if (manualState == LOW) {
    if (Water_level <= 10) {
      digitalWrite(LED_PIN, HIGH);
      servo.write(360);
    }

    else if (Water_level >= 8) {
      digitalWrite(LED_PIN, LOW);
      servo.write(0);
    }


  }

//LDR functionality
  int ldrStatus = analogRead(LDR_PIN);
  Serial.print("Light intensity:");
  Serial.println(ldrStatus);

  Serial.print("Water level: ");
  Serial.print(Water_level);
  Serial.println(" cm");
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.setCursor(0,0);
  lcd.print("Distance: ");
  lcd.print(Water_level);
  lcd.setCursor(0,1);
  lcd.print("Light Int: ");
  lcd.print(ldrStatus);

  publishMessage(sensor1_topic,String(Water_level),true);    
  publishMessage(sensor12_topic,String(ldrStatus),true);
  delay(1500);

}


//=======================================  
// This void is called every time we have a message from the broker

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
  
  //--- check the incomming message
    if( strcmp(topic,command1_topic) == 0){
     if (incommingMessage.equals("1")) digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on 
     else digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off 
  }

   //  check for other commands
 /*  else  if( strcmp(topic,command2_topic) == 0){
     if (incommingMessage.equals("1")) {  } // do something else
  }
  */


}

//========= publishing as string===============
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}
