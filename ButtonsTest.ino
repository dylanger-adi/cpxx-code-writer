#include <stdio.h>

#define INVPIN 2

#define W1PIN 3
#define W0PIN 4
#define CLRPIN 5
#define WRTPIN 6

#define LCDPIN 7
#define RCLPIN 8

#define ENPIN 9

#define LEDPIN 11

int incomingByte = 0; // for incoming serial data
int currentCode; //actual EN write code, from 0 to 63
bool newlineflag=false;
bool readValue;
char strbuf[20];
char entertm = 0b01111110;

void setup() {
  Serial.begin(38400); // opens serial port, sets data rate to 9600 bps
  Serial.println("\nSerial link established...");
  pinMode(INVPIN, INPUT);
  pinMode(W0PIN, INPUT);
  pinMode(W1PIN, INPUT);
  pinMode(CLRPIN, INPUT);
  pinMode(WRTPIN, INPUT);  
  pinMode(RCLPIN, INPUT);
  
}

void loop() { //when button pressed, returns all ROM Data
  Serial.print("INV:");
  Serial.print(digitalRead(INVPIN),BIN);
  Serial.print(" W0:");
  Serial.print(digitalRead(W0PIN),BIN);
  Serial.print(" W1:");
  Serial.print(digitalRead(W1PIN),BIN);
  Serial.print(" CLR:");
  Serial.print(digitalRead(CLRPIN),BIN);
  Serial.print(" WRT:");
  Serial.print(digitalRead(WRTPIN),BIN);
  Serial.print(" RCL:");
  Serial.println(digitalRead(RCLPIN),BIN);
  
  delay(250);
}
 
