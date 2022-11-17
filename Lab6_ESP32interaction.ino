#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define USE_SERIAL Serial
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */  
#define TIME_TO_SLEEP  15        /* Time ESP32 will go to sleep (in seconds) */      
RTC_DATA_ATTR int bootCount = 0;   //needed 
#include "Projectthml.h"

//WiFiMulti wifiMulti;
Servo myservo;
#define TRIG_PIN 33 // ESP32 pin GIOP33 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 34 // ESP32 pin GIOP34 connected to Ultrasonic Sensor's ECHO pin
#define LED_PIN  18 // LED pin set on/off based on water level
#define SDA_PIN 21 // SDA pin for LCD
#define SCL_PIN 22 // SCL pin for LCD
#define MOTOR_PIN 13 // Motor pin set on/off based on water level
#define MAX_DIST 60 //centimeters

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
float duration_us, distance_cm,Water_level;
int LEDR = LOW;
const char* ssid = "Uche";
const char* password = "ashesiclassof2023";
int manualState  = LOW;
//int counter = 0;

Servo servo;

WebServer server(80);

void print_wakeup_reason(){     ///needed
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  }

 
//  void baseHandle(){
//    
//    server.send(200, "text/plain","hello from esp32");
//    }

     void baseHandle(){
    
    server.send(200, "text/html",page);
    }

   

  void LEDRfunct()
{ if (manualState == HIGH){
  LEDR= !LEDR;
  digitalWrite(LED_PIN, LEDR);
  //counter++;
  String str = "OFF";    //very little returned
  if(LEDR == HIGH ) str = "ON";
  server.send(200, "text/plain", str);
  Serial.println("red");
}
}

void manualfunct(){
 if(manualState == HIGH){
   manualState = LOW;
}
 else{
  manualState = HIGH;
 }
 }
 
 
void setup() {
  // begin serial port
  Serial.begin (115200); 
  //pinMode(relay, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
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

  servo.attach(MOTOR_PIN);

  servo.write(0);


     for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
      //  delay(1000);
  
  }

  //wifiMulti.addAP("ssid","password");
  ++bootCount; 
   print_wakeup_reason();  
   esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");
  
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
 
    
     if (MDNS.begin("esp32")) {    //multicast DNS service
    Serial.println("MDNS responder started");
  }

  server.on("/", baseHandle );
    server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  server.on("/client", GET_RQ);

  server.begin();
  Serial.println("Http server started");
  server.on("/LEDRurl",LEDRfunct);
  server.on("/manualurl",manualfunct);

}
void loop(void) {   
  server.handleClient();
  delay(2);
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;


  Water_level = MAX_DIST - distance_cm;
 if (manualState == LOW){
 if (Water_level<=10){
    digitalWrite(LED_PIN,HIGH);
    servo.write(360);
  }
 
  else if (Water_level>=8){
    digitalWrite(LED_PIN,LOW);
    servo.write(0);
  }

  }

  lcd.setCursor(0,0);
  lcd.print("Water Level");
  lcd.setCursor(0,1);
  
  if (Water_level>=0){
    lcd.print(Water_level);
    lcd.setCursor(7,1);
    lcd.print("cm"); 
  }
  else{ // does not show negative depth
    lcd.print(Out of Bounds); 
  }
  
  
  
  Serial.print("Water level: ");
  Serial.print(Water_level); 
  Serial.println(" cm");
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  delay(1500);
  GET_RQ();

//// Normally Open configuration, send LOW signal to let current flow
//// (if you're usong Normally Closed configuration send HIGH signal)
//
//digitalWrite(relay, LOW);
//Serial.println("Current Flowing");
//delay(5000); 
//
//// Normally Open configuration, send HIGH signal stop current flow
//// (if you're usong Normally Closed configuration send LOW signal)
//digitalWrite(relay, HIGH);
//Serial.println("Current not Flowing");
//delay(5000);
}
String serverName = "http://192.168.137.1/iotinhtdocs/midsemproject/iotMidsemProject/Project_uche.php?"; 
void GET_RQ(){
  if((WiFi.status() == WL_CONNECTED)) {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        String serverPath = serverName + "Water_level="+String(Water_level);
 
        http.begin(serverPath.c_str()); //HTTP
  
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);
                server.send(200, "text/html", payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
  
 delay(500);
 Serial.println("Going to sleep now"); 
 //delay(2000);
  Serial.flush();                             
  esp_deep_sleep_start(); 
 }
