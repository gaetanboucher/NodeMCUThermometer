// include this library's description file
#include "Mcp3208.h"
#include <Arduino.h>

// include description files for other libraries used (if any)
#include "HardwareSerial.h"

static int selPin   = SELPIN;
static int dataOut  = DATAOUT;
static int dataIn   = DATAIN;
static int spiClock = SPICLOCK;  

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

Mcp3208::Mcp3208()
{
	// Initialize SPI to ADC 
	//set pin modes 
	pinMode(selPin, OUTPUT); 
	pinMode(dataOut, OUTPUT); 
	pinMode(dataIn, INPUT); 
	pinMode(spiClock, OUTPUT); 
	//disable device to start with 
	digitalWrite(selPin,HIGH); 
	digitalWrite(dataOut,LOW); 
	digitalWrite(spiClock,LOW); 
}

void Mcp3208::setCSpin(int csPin)
{
	selPin = csPin;
}

void Mcp3208::setDataOutpin(int dataOutPin)
{
	dataOut = dataOutPin;
}

void Mcp3208::setDataInpin(int dataInPin)
{
	dataIn = dataInPin;
}

void Mcp3208::setClock(int clock)
{
	spiClock = clock;
}

int Mcp3208::read(int channel){
  int adcvalue = 0;
  byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)

  //allow channel selection
  commandbits|=((channel-1)<<3);

  digitalWrite(SELPIN,LOW); //Select adc
  // setup bits to be written
  for (int i=7; i>=3; i--){
    digitalWrite(DATAOUT,commandbits&1<<i);
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);    
  }

  digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
  digitalWrite(SPICLOCK,LOW);
  digitalWrite(SPICLOCK,HIGH);  
  digitalWrite(SPICLOCK,LOW);

  //read bits from adc
  for (int i=11; i>=0; i--){
    adcvalue+=digitalRead(DATAIN)<<i;
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);
  }
  digitalWrite(SELPIN, HIGH); //turn off device
  return adcvalue;
}
