/*
 * Lumix Lamp v0.1
 * A mini interactive lamp.
 * This project is owned by Apurv, Himself, Raptor419 and The_Law.
 */

// Include Libraries
#include <Servo.h>

// Pin connections
#define servoLowerPin 2
#define servoUpperPin 3

// Objects 
Servo servoLower, servoUpper;

// Global Variables
int cmd = -1;

void setup() {
  Serial1.begin(9600); // Serial communication for Bluetooth
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  servoLower.attach(servoLowerPin);
  servoUpper.attach(servoUpperPin);
  servoLower.write(0); // Bring back to horizontal position. 
  servoUpper.write(180); // Bring back to horizontal position.
}

void loop() {
  // Read data from Bluetooth.
  if (Serial1.available() > 0) {
    cmd = Serial1.read();
    Serial.println("Something Received!");
  }

  // Take action after Bluetooth input.
  if(cmd != -1) {
    Serial.print("Data Received : ");
    Serial.println(cmd);
    if(cmd == 'A') {
      Serial1.println("A");  
    } else if(cmd == 'B') {
      Serial1.println("B");  
    } else if(cmd == 'C') {
      //  
    } else if(cmd == 'D') {
      //  
    } else if(cmd == 'E') {
      //  
    } else if(cmd == 'F') {
      //  
    } else if(cmd == 'G') {
      //  
    } else if(cmd == 'H') {
      //  
    } else if(cmd == 'I') {
      //  
    } else if(cmd == 'J') {
      //  
    } else if(cmd == 'K') {
      //  
    } else if(cmd == 'L') {
      //  
    }
    cmd = -1;
  }
}

// Functions for Modes.
void standardMode() {
    servoLower.write(30);
    delay(1000);
    servoUpper.write(90);
    delay(100); 
}

void vizualizerMode() {
    servoLower.write(135);
    delay(1000);
    servoUpper.write(45);
    delay(100); 
}
