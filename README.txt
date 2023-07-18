# cpxx-code-writer
*READ THIS FILE AS PLAIN TEXT*

Arduino software for one-off device that writes test mode data to cpxx projects

The device I made here is standalone and features a level shifter.  However, for reuse without the interface, the most important part of the code is writeBits(invState,writeBuf,bufLength).
  *invState should be 1 for CPxx products (normally high, active low).
  *writeBuf contains an unsigned int with up to 16 bits to write to the IC.
  *bufLength contains the number of bits you wish to write.
e.g. if you want to write "011001" to the EN pin:
  >writeBuf = 25 //0b0000000000011001 = 0x0019 = 25
  >bufLength = 6 //6 bits
  >writeBits(1,writeBuf,bufLength);
Keep timing definitions at top of code for well-behaved performance (tENHI, tENLO, TEN)

Note 1: uses a library to increase pin write speed: https://www.arduino.cc/reference/en/libraries/digitalpinfast/
Note 2: If you do not use a level shifter, your IO voltage for your Arduino must be greater than or equal to the supply voltage for the DUT.


//Timing diagram:
//
//"1":
//               EN high at tDETECT 
//______           _________¦________________________________________
//      |<--------tDETECT-->¦
//      |<-tENHI->|         ¦
//      |_________|         ¦
//"0":
//               EN low at tDETECT
//______                    ¦                             ___________
//      |<--------tDETECT-->¦                            |
//      |<-tENHI------------¦--------------------------> |
//      |___________________¦____________________________|
