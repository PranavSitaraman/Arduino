#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#define TCAADDR 0x70
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_16X);
int tcaselect(uint8_t i)
{
  if (i > 7)
  {
    return -4;
  }
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);  // 2^channel
  return (Wire.endTransmission());
}
void scan()
{
  for (int address = 1; address < 127; address++)
  {
    //Serial.println((int) address);
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    if (error==0)
    {
      //Serial.print("0x");
      //Serial.print(address, HEX);
      //Serial.print('\t');
      //Serial.println(error);
    }
  }
}
void check()
{
  //Serial.println("Scanner Begin");
  for (byte channel = 0; channel < 8; channel++)
  {
    //Serial.print("Staring channel:");
    //Serial.println(channel);
    if (tcaselect(channel) != 0)
    {
      //Serial.print("MUX is down at");
      //Serial.println(TCAADDR,HEX);
      //while (1);
    }
    //delay(100);
    scan();
  }
  //delay(2000); 
}
void setup(){
  Serial.begin(9600);
  Wire.begin();
  tcs.begin(0x70);
  check();
}
void loop()
{
  float r, g, b;
  tcaselect(1);
  tcs.getRGB(&r, &g, &b);
  Serial.print("R: "); Serial.print(int(r)); Serial.print("; ");
  Serial.print("G: "); Serial.print(int(g)); Serial.print("; ");
  Serial.print("B: "); Serial.print(int(b)); Serial.println(" ");
  tcaselect(2);
  tcs.getRGB(&r, &g, &b);
  Serial.print("R: "); Serial.print(int(r)); Serial.print("; ");
  Serial.print("G: "); Serial.print(int(g)); Serial.print("; ");
  Serial.print("B: "); Serial.print(int(b)); Serial.println(" ");
  Serial.println(" ");
}
