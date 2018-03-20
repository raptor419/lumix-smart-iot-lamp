#include <LiquidCrystal.h> //Load Liquid Crystal Library
#include <WiFiClient.h>
LiquidCrystal LCD(10, 9, 5, 4, 3, 2);  //Create Liquid Crystal Object called LCD
int tempPin = 0;
int temp = 0;
#include<Wire.h>
#define rtc 0x68
/*rtc stores data in bcd

*/
String getTime() {
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) {
    Serial.println("connection failed, retrying...");
  }
  
  client.print("HEAD / HTTP/1.1\r\n\r\n");
  while(!!!client.available()) {
     yield();
  }

  while(client.available()){
    if (client.read() == '\n') {    
      if (client.read() == 'D') {    
        if (client.read() == 'a') {    
          if (client.read() == 't') {    
            if (client.read() == 'e') {    
              if (client.read() == ':') {    
                client.read();
                String theDate = client.readStringUntil('\r');
                client.stop();
                return theDate;
              }
            }
          }
        }
      }
    }
  }
}
byte bcdtodec(byte val)
{
  return ((val/16*10)+(val%16));
}
byte dectobcd(byte val)
{
  return ((val/10*16)+(val%10));
}
void readtime(byte *secondd, byte *minutee, byte *hourr, byte *dayy, byte *datee, byte *monthh, byte *yearr)
{
  //to read time from rtc module
  //you have to read time from specific registers, hence use addresses and pointers
  //secondd, minutee,etc are pointers to registers
  Wire.beginTransmission(rtc);//begins transmitting data to rtc module
  Wire.write(0);//to read something we have to send a dummy write signal
  Wire.endTransmission();
  Wire.requestFrom(rtc,7);//request 7 bytes of data as 7 variables
  *secondd=bcdtodec(Wire.read()& 0x7f);
  *minutee=bcdtodec(Wire.read());
  *hourr=bcdtodec(Wire.read()& 0x3f);
  *dayy=bcdtodec(Wire.read());
  *datee=bcdtodec(Wire.read());
  *monthh=bcdtodec(Wire.read());
  *yearr=bcdtodec(Wire.read());
  
}
void settime(byte secondd, byte minutee, byte hourr, byte dayy, byte datee, byte monthh, byte yearr)
{
  Wire.beginTransmission(rtc);
  Wire.write(0);
  //whenever you have to communicate with the board, you have to write the above two commands
  Wire.write(dectobcd(secondd));
  Wire.write(dectobcd(minutee));
  Wire.write(dectobcd(hourr));
  Wire.write(dectobcd(dayy));
  Wire.write(dectobcd(datee));
  Wire.write(dectobcd(monthh));
  Wire.write(dectobcd(yearr));
  Wire.endTransmission();
  
}
void displaytime()
{
  byte secondd,minutee,hourr,dayy,datee,monthh,yearr;
  readtime(&secondd,&minutee,&hourr,&dayy,&datee,&monthh,&yearr);
  Serial.print(hourr);
  Serial.print(":");
  Serial.print(minutee);
  Serial.print(":");
  Serial.print(secondd);
  Serial.println();
  Serial.print("Day: "+dayy);
  Serial.println();
  Serial.print(datee);
  Serial.print("/");
  Serial.print(monthh);
  Serial.print("/");
  Serial.println(yearr);
}
//I2 protocol where rtc is a slave it has 68(in hexa) as it's address.
void setup() {
  LCD.begin(16, 2); //Tell Arduino to start your 16 column 2 row LCD
  LCD.setCursor(0, 0); //Set LCD cursor to upper left corner, column 0, row 0
  LCD.print("Temperature : ");  //Print Message on First Row
  Wire.begin();
  Serial.begin(9600);
  settime(00,25,15,03,02,02,18);
}

void loop() {
  Serial.println(getTime());
  temp = analogRead(tempPin) * 0.48828125;
  LCD.setCursor(0, 1); //Go to 1st column(column 0) and 2nd row(row 1)
  LCD.print("                "); //Print blanks to clear the row
  LCD.setCursor(0, 1); //Go to 1st column(column 0) and 2nd row(row 1)
  LCD.print(temp);
  LCD.print(" Celcius");
  displaytime();
  delay(1000);
}
