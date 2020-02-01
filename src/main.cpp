#include <Arduino.h>
#include <Mcp3208.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <Thermometer.h>

/* Set these to your desired credentials. */
const char *ssid = "MonNodeMCU";
const char *password = "1234";

ESP8266WebServer server(80);
float probe1=0, probe2=0;

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
   server.send(200, "text/html", 
    "<HEAD><TITLE>WIFI Thermometer</TITLE><META HTTP-EQUIV='refresh' CONTENT='5'></HEAD><h1 style='font-family:verdana;font-size:50px;text-align:center;color:blue;'>Probe 1: " + String(probe1) + "&#8451;</h1><h1 style='font-family:verdana;font-size:50px;text-align:center;color:red;'>Probe 2: " + String(probe2)+ "&#8451;</h1>");
}

Mcp3208 adc;
float getTemp(int Vo);

void setup(){ 
  Serial.begin(115200); 
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
} 

int totalSamples = 0;
int samples = 0;

void loop() { 
  int pin1, pin2;
  server.handleClient();
  int average = 0;
  pin1 = adc.read(1);
  pin2 = adc.read(2);
  if (samples > 1) 
  {
    average = totalSamples / samples;
  }
  Serial.print("Pin1: "); 
  Serial.print(pin1,DEC); 
  Serial.print(", Temp="); 
  probe1 = getTemp(pin1);
  Serial.print(probe1); 
  Serial.print(", Pin2: "); 
  Serial.print(pin2,DEC); 
  Serial.print(", Temp="); 
  probe2 = getTemp(pin2);
  Serial.print(probe2); 
  Serial.println(); 
  samples++;
  totalSamples += pin1;
  delay(1000); 
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