//Dylanger Gutierrez, 2022-06-21
//Version 0.2
//Added functions to read buttons and drive output pin.
//
//This is the code to drive the box to control the interface for CP parts.  Normally high, where 1 and 0 are determined by negative width.

#include <stdio.h>
//define necessary pins
#define INVPIN 2

#define W0PIN 3
#define W1PIN 4
#define CLRPIN 5
#define WRTPIN 6

#define LCDPIN 7

#define ENPIN 9

#define LEDPIN 11

#define tENHI 1     //in us
#define tENLO 100   //in us
#define TEN 500     //in us. Period must be longer than tENLO

bool invertOut=false;
bool invSwitchState=false;
int buflength=0;
unsigned int writebuf=0; //should be 16 bit
int buttonState=0;

void setup() {
  pinMode(INVPIN, INPUT); //Switch to control if the box is normally high or low
  pinMode(W0PIN, INPUT);  //button; adds a '0' to the buffer
  pinMode(W1PIN, INPUT);  //button; adds a '1' to the buffer
  pinMode(CLRPIN, INPUT); //button; clears write buffer
  pinMode(WRTPIN, INPUT); //button; writes buffer to the output

  pinMode(ENPIN, OUTPUT); //Drives with digital output signal
  pinMode(LEDPIN, OUTPUT);//Blinks when buffer written
  digitalWrite(ENPIN,LOW);
  digitalWrite(LEDPIN,LOW);
  
}

void loop() {
  invSwitchState = digitalRead(INVPIN);
  if(invertOut & (!invSwitchState)) { //non-inverting case if switch is off
    invertOut=false;
    digitalWrite(ENPIN,LOW);
  }
  if((!invertOut) & invSwitchState) { //inverting case if switch is on; normal mode of operation
    invertOut=true;
    digitalWrite(ENPIN,HIGH);
  }
  buttonState = readButtons(); //Button State: 0= no read; 1= "W0"; 2= "W1"; 3="CLR"; 4="WRT"
  if((buttonState==1) & buflength<16) {
    writebuf = writebuf<<1; //shifts left and leaves lsb=0
    buflength++;    
  }
  if((buttonState==2) & buflength<16) {
    writebuf = (writebuf<<1) + 1; //shifts left and leaves lsb=0
    buflength++;
  }
  if(buttonState==3) {
    writebuf = 0;
    buflength = 0;
  }
  if(buttonState==4) {
    writeBits(invertOut,writebuf,buflength);
  }
  if(buttonState>0) {
    
  }
  updateScreen(buttonState,writebuf,buflength);
}

int readButtons() {       //checks which button pressed
  if(digitalRead(W0PIN)) {
    delay(50);
    while(digitalRead(W0PIN));
    delay(50);
    return 1;
  }
  if(digitalRead(W1PIN)) {
    delay(50);
    while(digitalRead(W1PIN));
    delay(50);
    return 2;
  }
  if(digitalRead(CLRPIN)) {
    delay(50);
    while(digitalRead(CLRPIN));
    delay(50);
    return 3;
  }
  if(digitalRead(WRTPIN)) {
    delay(50);
    while(digitalRead(WRTPIN));
    delay(50);
    return 4;
  }
  return 0;
}

void writeBits(bool invState,unsigned int wbuf,int blen) {  //writes bits to output
  if(invState) {
    for(int k=blen-1;k>=0;k--) {
      if(wbuf & (1<<k)) {
        digitalWrite(ENPIN, LOW);
        delayMicroseconds(tENHI);
        digitalWrite(ENPIN, HIGH);
        delayMicroseconds(TEN-tENHI);
      } else {
        digitalWrite(ENPIN, LOW);
        delayMicroseconds(tENLO);
        digitalWrite(ENPIN, HIGH);
        delayMicroseconds(TEN-tENLO);
      }
    }
  } else {
    for(int k=blen-1;k>=0;k--) {
      if(wbuf & (1<<k)) {
        digitalWrite(ENPIN, HIGH);
        delayMicroseconds(tENHI);
        digitalWrite(ENPIN, LOW);
        delayMicroseconds(TEN-tENHI);
      } else {
        digitalWrite(ENPIN, HIGH);
        delayMicroseconds(tENLO);
        digitalWrite(ENPIN, LOW);
        delayMicroseconds(TEN-tENLO);
      }
    }
  }
  return;
}

void updateScreen(int buttonState,unsigned int writebuf,int buflength) {
  return;
}
