#include <Arduino.h>
#include <Mcp3208.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <CaptivePortalAdvanced.h>
//#include <Thermometer.h>

float probe1=0, probe2=0;

Mcp3208 adc;
float getProbeTemp(int probeId);
float getTemp(int Vo);

void setup(){ 
  Serial.begin(115200); 
  setupPortal();
} 

int totalSamples = 0;
int samples = 0;

void loop() { 
  int pin1, pin2;
  pin1 = adc.read(1);
  pin2 = adc.read(2);
  // Serial.print("Pin1: "); 
  // Serial.print(pin1,DEC); 
  // Serial.print(", Temp="); 
  probe1 = getProbeTemp(pin1);
  // Serial.print(probe1); 
  // Serial.print(", Pin2: "); 
  // Serial.print(pin2,DEC); 
  // Serial.print(", Temp="); 
  probe2 = getTemp(pin2);
  // Serial.print(probe2); 
  // Serial.println(); 
  samples++;
  totalSamples += pin1;
  loopPortal();
} 

float getProbeTemp(int probeId) { 
  float probe=0;
  int pin = probeId;
  int average = 0;
  pin = adc.read(probeId);
  probe = getTemp(pin);
} 

float Vin= 3.3;
float Vout= 0;
float R1= 22000;
float R2= 0;
float buffer= 0;

float secondResistance(int raw)
{
  if(raw) 
  {
    buffer= raw * Vin;
    Vout= (buffer)/4095.0;
    buffer= (Vin/Vout) -1;
    R2= R1 * buffer;
  }
  return R2;
}

float getTemp(int Vo) 
{
  float R1 = 47000;
  float logR2, R2, T;
  float A = 0.9331787057e-03, B = 1.446098516e-04, C = 1.604049552e-07;  // Steinhart-Hart and Hart Coefficients

  R2 = R1* (4095.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (A + B*logR2 + C*logR2*logR2*logR2));  // Steinhart and Hart Equation. T  = 1 / {A + B[ln(R)] + C[ln(R)]^3}
  T =  T - 273.15;

  return T;
}