#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

#define JB1 
#define JB2
#define CH1
#define CH2
#define PB1
#define PB2
#define PB3
#define PB4

RF24 radio(5, 6);   // nRF24L01 (CE, CSN)
const byte address[6] = "10101"; // Address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct dataPackage
{
  byte joy1potX;
  byte joy1potY;
  byte joy2potX;
  byte joy2potY;
  byte joy1pb;
  byte joy2pb;
  byte ch1;
  byte ch2;
  byte pot1;
  byte pot2;
  byte pb1;
  byte pb2;
  byte pb3;
  byte pb4;
};

dataPackage data;


void setup() {
  Serial.begin(9600);

  // Defining the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  // Actavating internal pull-up resistor
  pinMode(JB1,INPUT_PULLUP);
  pinMode(JB2,INPUT_PULLUP);
  pinMode(CH1,INPUT_PULLUP);
  pinMode(CH2,INPUT_PULLUP);
  pinMode(PB1,INPUT_PULLUP);
  pinMode(PB2,INPUT_PULLUP);
  pinMode(PB3,INPUT_PULLUP);
  pinMode(PB4,INPUT_PULLUP);

 // Setting default values

 data.joy1potX=127;
 data.joy1potY=127;
 data.joy2potX=127;
 data.joy2potY=127;
 data.joy1pb=1;
 data.joy2pb=1;
 data.ch1=1;
 data.ch2=1;
 data.pot1=1;
 data.pot2=1;
 data.pb1=1;
 data.pb2=1;
 data.pb3=1;
 data.pb4=1;
}

void loop() {

  // Reading analog values and Converting the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.joy1potX = map(analogRead(/*insert pin no*/), 0, 1023, 0, 255); 
  data.joy1potY = map(analogRead(/*insert pin no*/), 0, 1023, 0, 255);
  data.joy2potX = map(analogRead(/*insert pin no*/), 0, 1023, 0, 255);
  data.joy2potY = map(analogRead(/*insert pin no*/), 0, 1023, 0, 255);
  data.pot1 = map(analogRead(/*insert pin no*/), 0, 1023, 0, 255);
  data.pot2 = map(analogRead(/*insert pin no*/), 0, 1023, 0, 255);

  //Reading digital values
  data.ch1=digitalRead(/*insert pin no*/);
  data.ch2=digitalRead(/*insert pin no*/);
  data.joy1pb=digitalRead(/*insert pin no*/);
  data.joy2pb=digitalRead(/*insert pin no*/);
  data.pb1=digitalRead(/*insert pin no*/);
  data.pb2=digitalRead(/*insert pin no*/);
  data.pb3=digitalRead(/*insert pin no*/);
  data.pb4=digitalRead(/*insert pin no*/);

  // Sending the whole data from the structure to the receiver
  radio.write(&data, sizeof(dataPackage));

}
