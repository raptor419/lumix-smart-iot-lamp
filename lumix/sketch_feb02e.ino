

#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>

LiquidCrystal LCD(12, 11, 7, 6, 5, 4);
int tempPin=A0;
int temp=0;
RTC_DS1307 RTC;

void setup () {
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

}

void loop () {
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.println();
    LCD.begin(16, 2); //Tell Arduino to start your 16 column 2 row LCD
    LCD.setCursor(0, 0); //Set LCD cursor to upper left corner, column 0, row 0
    LCD.print(now.year(), DEC);
    LCD.print('/');
    LCD.print(now.month(), DEC);
    LCD.print('/');
    LCD.print(now.day(), DEC);
    LCD.print(' ');
    LCD.print(now.hour(), DEC);
    LCD.print(':');
    LCD.print(now.minute(), DEC);
    temp = analogRead(tempPin) * 0.48828125;
    LCD.setCursor(0, 1); //Go to 1st column(column 0) and 2nd row(row 1)
    LCD.print(temp);
    LCD.print(" Celcius");
    Wire.begin();
    Serial.begin(9600);
    delay(3000);
}
