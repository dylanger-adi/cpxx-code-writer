//Dylanger Gutierrez, 2022-07-01 (previous date on 0.4 was incorrect)
//Version 1.0
//Initial usable release.  
//
//This is the code to drive the box to control the interface for CP parts.  Normally high, where 1 and 0 are determined by negative width.

//junk for the LCD Screen
#include "Arduino.h"
#if defined(ARDUINO_ARCH_SAMD) || defined(__SAM3X8E__)
  // use pin 18 with Due, pin 1 with Zero or M0 Pro 
  #define lcd Serial1 
#else
  #include <SoftwareSerial.h>
  // Create a software serial port!
  SoftwareSerial lcd = SoftwareSerial(0,7); //pin 7 is LCD
#endif

#include <stdio.h>
//define necessary pins
#define INVPIN 2

#define W0PIN 4
#define W1PIN 3
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

  //Below is LCD Stuff
  lcd.begin(9600);  
  
  // set the size of the display if it isn't 16x2 
  lcd.write(0xFE);
  lcd.write(0xD1);
  lcd.write(16);  // 16 columns
  lcd.write(2);   // 2 rows
  delay(10);       

  // set the contrast, 200 is a good place to start, adjust as desired
  lcd.write(0xFE);
  lcd.write(0x50);
  lcd.write(200);
  delay(10);       
  
  // set the brightness 
  lcd.write(0xFE);
  lcd.write(0x99);
  lcd.write(255);
  delay(10);       
  
  // turn off cursors
  lcd.write(0xFE);
  lcd.write(0x4B);
  lcd.write(0xFE);
  lcd.write(0x54);
  delay(10);

   // scroll behavior
  lcd.write(0xFE);
  lcd.write(0x52);
  delay(10);

  //writes splash screen
  lcd.write(0xFE);
  lcd.write(0x40);
  lcd.write("Code Writer v1.0by Dylanger G.  ");//32 chars; version number
  delay(10);

  // clear screen
  lcd.write(0xFE); 
  lcd.write(0x58);
  delay(10);

  //set backlight neutral
  writeBacklightColor(63,63,63);

  //splash screen initial text
  lcd.write("Code Writer v1.0by Dylanger G.  ");//32 chars; version number

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
    updateScreen(buttonState,writebuf,buflength);
  }
  
//  if(buttonState==4) {  //not currently used but could be added
//    for(int i=0;i<3;i++) {
//      digitalWrite(LEDPIN,HIGH);
//      delay(100);
//      digitalWrite(LEDPIN,LOW);
//      delay(30);
//    }
//  }
  buttonState=0;
  delay(10);
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
        //delayMicroseconds(tENHI); too slow already
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
        //delayMicroseconds(tENHI);
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

void updateScreen(int buttonState,unsigned int writebuf,int blen) {
  char buftext[17];
  buftext[16]='\0'; //set null character
  for(int k=0; k<(16-blen);k++) { //fill leader with spaces
    buftext[15-k]=' ';
  }
  for(int k=0;k<blen;k++) { //fill leader with spaces
    if((writebuf>>(blen-1-k)) & 1) { //puts a '1' for 1 bits and a '0' for 0
      buftext[k]='1';
    } else {
      buftext[k]='0';
    }
  }
  for(int k=17;k>=blen;k--) { //initialize both lines of text to NULL
    buftext[k]='\0';
  }
  lcd.write(0xFE); // clear screen
  lcd.write(0x58);
  delay(10);
  lcd.write(0xFE); // go 'home' to origin
  lcd.write(0x48);
  delay(10);
  if(buttonState==1 ) { //W0 or W1 case
    lcd.print("Bits to write:  ");
    lcd.write(0xFE); // move write position
    lcd.write(0x47);
    lcd.write(16-blen+1);//position in row
    lcd.write(2);//second column
    delay(10);
    lcd.print(buftext);
    delay(10);
    writeBacklightColor(63,63,255); //Blue
  } else if(buttonState==2) {
    lcd.print("Bits to write:  ");
    lcd.write(0xFE); // move write position
    lcd.write(0x47);
    lcd.write(16-blen+1);//position in row
    lcd.write(2);//second column
    delay(10);
    lcd.print(buftext);
    delay(10);
    writeBacklightColor(255,100,0); //yellow
  } else if(buttonState==3) {
    lcd.print("Buffer Cleared. ");
    delay(10);
    writeBacklightColor(255,20,20); //red
  } else if((buttonState==4)&(blen>0)) {
    writeBacklightColor(230,50,50); //pink
    delay(100);
    lcd.print("Written Bits:   ");
    lcd.write(0xFE); // move write position
    lcd.write(0x47);
    lcd.write(16-blen+1);//position in row
    lcd.write(2);//second column
    delay(10);
    lcd.print(buftext);
    delay(10);
    writeBacklightColor(20,255,20); //green
  }    
  //lcd.print(buttonState,DEC); //diagnostic
  delay(100);
  return;
}

void writeBacklightColor(byte R, byte G, byte B) {
  lcd.write(0xFE);
  lcd.write(0xD0);
  lcd.write(R);
  lcd.write(G);
  lcd.write(B);
  return;
}
