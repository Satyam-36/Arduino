#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(10, 9);   // nRF24L01 (CE, CSN)
const byte address[6] = "10101";

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
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  //Setting the module as receiver
  radio.startListening(); 
  //Reseting the data if no data is there to recieve
  resetData();
}

void resetData()
{
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
    // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(dataPackage)); // Reading the whole data and store it into the 'data' structure
  }
  else{
    resetData();
  }

}
