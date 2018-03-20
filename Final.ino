/*
   Lumix Lamp v0.2
   A smart IOT interactive lamp.
   This project is owned by Apurv, Himself, Raptor419 and The_Law.
*/

// Include Libraries
#include <Servo.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include <english.h>
#include <TTS.h>
#include <LedControl.h>

// Pin connections
#define tempPin A0
#define servoLowerPin 7
#define servoUpperPin 6
#define BtSerial Serial1
#define EspSerial Serial2
#define ESP8266_BAUD 9600
#define BLYNK_PRINT Serial
#define testPin 13
#define leddin 8
#define ledcs 9
#define ledclk 10
#define ledno 1
#define soundPin 3
#define lcdrs 12
#define lcde 11
#define db4 36
#define db5 34
#define db6 32
#define db7 30
#define servodelay 100
#define speech 2
#define lamp 4

//Variables

// Visuals
byte o1[8]= {0x3c,0x42,0x99,0xa5,0xa5,0x99,0x42,0x3c};
byte o2[8]= {0x00,0x3c,0x42,0x5a,0x5a,0x42,0x3c,0x00};

// Auth Token
char auth[] = "0f9599e3dce0406295d093ee9e074e52";

// WiFi credentials.
char ssid[] = "Honor7x";
char pass[] = "88888888";

// Objects
Servo servoLower, servoUpper;
LiquidCrystal lcd(lcdrs, lcde, db4, db5, db6, db7);
ESP8266 wifi(&EspSerial);
RTC_DS1307 rtc;
TTS text2speech(speech);
LedControl lc=LedControl(leddin,ledclk,ledcs,ledno);


// Global Variables
int cmd = -1;
int mode = 1;
int temp = 0;
char text [50];
int state = 1;
int soundState = 0;
int servoLowerAngle = 180;
int servoUpperAngle = 0;
bool isFirstConnect = true;

void setup() {
  lc.shutdown(0,false);       
  lc.setIntensity(0,15);
  lc.clearDisplay(0);
  Serial.begin(9600); // Serial communication for Blynk
  Serial1.begin(9600); // Serial communication for Bluetooth
  Serial2.begin(9600); // Serial communication for WiFi
  Wire.begin();
  rtc.begin();
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Set at first
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  delay(10);
  pinMode(testPin, OUTPUT);
  Blynk.begin(auth, wifi, ssid, pass);
  servoLower.attach(servoLowerPin);
  servoUpper.attach(servoUpperPin);
  servoLowerAngle = servoLower.read();
  servoUpperAngle = servoUpper.read();
  // Close the lamp, don't remove this from here!
  if (servoUpperAngle != 180) {
    servoUpperAngle++;
    while (servoUpperAngle != 180) {
      servoUpper.write(servoUpperAngle);
      servoUpperAngle++;
      delay(20);
    }
  }
  if (servoLowerAngle != 180) {
    servoLowerAngle++;
    while (servoLowerAngle != 180) {
      servoLower.write(servoLowerAngle);
      servoLowerAngle++;
      delay(20);
    }
  } 
  delay(1000);
}
void loop() {
  //Blynk Run
  Blynk.run();
  //date time temp
  DateTime now = rtc.now();
  lcd.begin(16, 2); //Tell Arduino to start your 16 column 2 row LCD
  lcd.setCursor(0, 0); //Set LCD cursor to upper left corner, column 0, row 0
  lcd.print(now.year(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  lcd.print(' ');
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  temp = analogRead(tempPin) * 0.48828125;
  lcd.setCursor(0, 1); //Go to 1st column(column 0) and 2nd row(row 1)
  lcd.print(temp);
  lcd.print(" Celcius");
  delay(10);
  
  if(state==0){
    closeLamp();
  }
  if(mode==1){
    visDisplay();
  }
}

// Functions for Modes.
void standardMode() {
  if (servoLowerAngle != 80) {
    int gap;
    if (servoLowerAngle > 80) {
      gap = -1;
    } else {
      gap = 1;
    }
    servoLowerAngle = servoLowerAngle + gap;
    while (servoLowerAngle != 80) {
      servoLower.write(servoLowerAngle);
      servoLowerAngle += gap;
      delay(10);
    }
  }
  if (servoUpperAngle != 20 ) {
    int gap;
    if (servoUpperAngle > 20) {
      gap = -1;
    } else {
      gap = 1;
    }
    servoUpperAngle+=gap;
    while (servoUpperAngle != 20) {
      servoUpper.write(servoUpperAngle);
      servoUpperAngle+=gap;
      delay(10);
    }
  }
  delay(1000);
}

void visDisplay(){
  soundState = digitalRead (soundPin);
   if(true){
       digitalWrite (testPin, HIGH);
       printByte(o1);     
       delay(100);
       digitalWrite (testPin, LOW);
       printByte(o2);     
       delay(100);
       Serial.println (soundState, DEC);
   }
   Serial.println (soundState, DEC);
   lc.clearDisplay(0);
}


void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}

void Test_Speech() {
  text2speech.setPitch(6); //higher values = lower voice pitch
  strcpy(text, "Hello  master!");
  text2speech.sayText(text);
  delay(500);
}

void closeLamp(){
  if (servoUpperAngle != 180) {
    servoUpperAngle++;
    while (servoUpperAngle != 180) {
      servoUpper.write(servoUpperAngle);
      servoUpperAngle++;
      delay(20);
    }
  }
  if (servoLowerAngle != 180) {
    servoLowerAngle++;
    while (servoLowerAngle != 180) {
      servoLower.write(servoLowerAngle);
      servoLowerAngle++;
      delay(20);
    }
  } 
  delay(1000);
}

BLYNK_WRITE(V0){
  int state = param.asInt();
  if (state==1){
  standardMode();
  }
  else if(state==0){
    closeLamp();
  }
}

BLYNK_WRITE(V1){
  int m = param.asInt();
  if (m==1){
    mode=1;
  }
  else if(state==0){
    mode=0;
  }
}

BLYNK_CONNECTED() // runs every time Blynk connection is established
{
    if (isFirstConnect) 
    {
// Request server to re-send latest values for all pins
    Blynk.syncAll();
    isFirstConnect = false;
    }
}

