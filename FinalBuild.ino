/*
   Lumix Lamp v0.1
   A mini interactive lamp.
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
#define db4 28
#define db5 26
#define db6 24
#define db7 22
#define servodelay 100
#define speech 2
#define lamp 52

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
int mode = 0;
int temp = 0;
char text [50];
boolean state = 0;
int soundState = 0;
int servoLowerAngle = 180;
int servoUpperAngle = 0;

//Setup
void setup() {
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
  if (servoUpperAngle != 0) {
    servoUpperAngle--;
    while (servoUpperAngle != 0) {
      servoUpper.write(servoUpperAngle);
      servoUpperAngle--;
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

//Loop
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
  digitalWrite(53,mode);
  // Display visuals
  if(mode==1){
    visDisplay();
  }
  
  
  
  //BTRead
  if (BtSerial.available() > 0) {
    cmd = BtSerial.read();
    Serial.println("Something Received!");
  }

  // Take action after Bluetooth input.
  if (cmd != -1) {
    Serial.print("Data Received : ");
    Serial.println(cmd);
    if (cmd == 'A') {
      Serial1.println("A");
    } else if (cmd == 'B') {
      Serial1.println("B");
    } else if (cmd == 'C') {
      Serial1.println("C");
      digitalWrite(lamp,HIGH);
    } else if (cmd == 'D') {
      Serial1.println("D");
      digitalWrite(lamp,LOW);
    } 
    
    cmd = -1;
  }
}

// Functions

//Modes
// Functions for Modes.
void standardMode() {
  if (servoLowerAngle != 150) {
    int gap;
    if (servoLowerAngle > 150) {
      gap = -1;
    } else {
      gap = 1;
    }
    servoLowerAngle = servoLowerAngle + gap;
    while (servoLowerAngle != 60) {
      servoLower.write(servoLowerAngle);
      servoLowerAngle += gap;
      delay(10);
    }
  }
  if (servoUpperAngle != 180) {
    servoUpperAngle++;
    while (servoUpperAngle != 180) {
      servoUpper.write(servoUpperAngle);
      servoUpperAngle++;
      delay(10);
    }
  }
  delay(1000);
}

void visualiserMode() {
  if (servoLowerAngle != 60) {
    int gap;
    if (servoLowerAngle > 60) {
      gap = -1;
    } else {
      gap = 1;
    }
    servoLowerAngle = servoLowerAngle + gap;
    while (servoLowerAngle != 60) {
      servoLower.write(servoLowerAngle);
      servoLowerAngle += gap;
      delay(10);
    }
  }
  if (servoUpperAngle != 180) {
    servoUpperAngle++;
    while (servoUpperAngle != 180) {
      servoUpper.write(servoUpperAngle);
      servoUpperAngle++;
      delay(10);
    }
  }
  delay(1000);
}

void closeLamp() {
  if (servoUpperAngle != 0) {
    servoUpperAngle--;
    while (servoUpperAngle != 0) {
      servoUpper.write(servoUpperAngle);
      servoUpperAngle--;
      delay(10);
    }
  }
  if (servoLowerAngle != 180) {
    servoUpperAngle++;
    while (servoUpperAngle != 180) {
      servoUpper.write(servoUpperAngle);
      servoUpperAngle++;
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
