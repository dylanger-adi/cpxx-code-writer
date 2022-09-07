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

#define tENHI 1     //in us
#define tENLO 100   //in us

bool invertOut=false;
int buflength=0;
unsigned int writebuf=0; //should be 16 bit

void setup() {
  // put your setup code here, to run once:
  

}

void loop() {
//invertswitchstate = read(norm Hi Switch)
//if(invertOut & !invertswitchstate)
//  invertswitchstate=true
//  write output high
//else if (!invertOut & invertswitchstate)
//  invertswitchstate=false
//  write output low
//buttonState=readButtons() //Button State: 0= no read; 1= "W0"; 2= "W1"; 3="CLR"; 4="WRT"
//
//if (W0 and buflength<16):
//  Shift buffer to the left one bit
//  length++
//else if (W1 and buflength<16):
//  Shift buffer to the left one bit and add 1
//  length++
//else if CLR:
//  writebuf=0
//  buflength=0
//else if WRITE:
//  writeBits(invertState,writebuf,buflength)
//  writebuf=0
//  buflength=0
//if buttonState>0:
//  updateScreen(buttonState,writebuf,buflength)
//
//
}

int readButtons() {
  
}
