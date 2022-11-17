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
float duration_us, distance_cm,Water_level;
int LEDR = LOW;
const char* ssid = "Uche";
const char* password = "ashesiclassof2023";
int manualState  = LOW;
//int counter = 0;

Servo servo;

WebServer server(80);

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




void setup() {
  // put your setup code here, to run once:

  // begin serial port
  Serial.begin (115200);
  
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


}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2);
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

  
  lcd.print("Distance: ");
  lcd.print(Water_level);
//  lcd.setCursor(1, 0);
//  lcd.print("Light Int: ");
//  lcd.print(ldrStatus);



  delay(1500);
}