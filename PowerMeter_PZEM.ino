/*
- CONNECTIONS: nRF24L01 Modules See:
 http://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
 */


#include <JeeLib.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

// NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xF0F0F0F0D2LL; // Define the transmit pipe

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void setup() {
  
  Serial.begin(9600);
  
    radio.begin();
  radio.openWritingPipe(pipe);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //Set address B4 C0 A8 01 02 00 1F
  printCommand(180);
  printCommand(192);
  printCommand(168);
  printCommand(1);
  printCommand(2);
  printCommand(0);
  printCommand(31);
  delay(1000);
}

void loop() { // run over and over

  int powerMeasure;

  //Get Current B1 C0 A8 01 02 00 1C
  printCommand(177);
  printCommand(192);
  printCommand(168);
  printCommand(1);
  printCommand(2);
  printCommand(0);
  printCommand(28);

  delay(1000);
  
  if(Serial.available()){
    
    int j = 0;
    String part1;
    String part2;
    while(Serial.available()) {
      
      byte result= Serial.read();
      
      if(j==2){
        
       part1 = (String)result;

      }
      if(j==3){

       part2 = (String)result;
       
       String currentValue = part1 + "." + part2;
       float powerValue = currentValue.toFloat()*235;
  
       powerMeasure = (int)powerValue;

       //Send over wifi
      radio.write(&powerMeasure, sizeof(int));
      
     }
     j++;
    
      

    }
  }
  delay(3000);
    //Sleep for 3s
  //    Sleepy::loseSomeTime(3000);

}

void printCommand(char toprint){
  Serial.print(toprint);
}

/*
void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
} 

unsigned concatenate(unsigned x, unsigned y) {
    unsigned pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;        
}

unsigned int hexToDec(String hexString) {
  
  unsigned int decValue = 0;
  int nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
  return decValue;
}*/
