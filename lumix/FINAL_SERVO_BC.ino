/*
   Lumix Lamp v0.1
   A mini interactive lamp.
   This project is owned by Apurv, Himself, Raptor419 and The_Law.
*/

// Include Libraries
#include <Servo.h>

// Pin connections
#define servoLowerPin 6
#define servoUpperPin 7

// Objects
Servo servoLower, servoUpper;

// Global Variables
int cmd = -1;
int mode = -1;
int servoLowerAngle = 180;
int servoUpperAngle = 0;

void setup() {
  //  Serial1.begin(9600); // Serial communication for Bluetooth
  Serial.begin(9600);
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

void loop() {

}

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
