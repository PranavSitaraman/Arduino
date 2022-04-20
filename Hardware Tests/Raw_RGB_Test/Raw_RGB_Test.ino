#include <Wire.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);
#define RIGHT 7
#define LEFT 5
void setup(void)
{
  Serial.begin(9600);
  Wire.begin();
  int err = tselect(RIGHT);
  Serial.println(err);
  if (tcs.begin())
  {
    Serial.println("Found sensor");
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  err = tselect(LEFT);
  Serial.println(err);
  if (tcs.begin())
  {
    Serial.println("Found sensor");
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}
int tselect(uint8_t i)
{
  if (i > 7)
  {
    return -4;
  }
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  return (Wire.endTransmission());
}
void loop(void)
{
  if (Serial.available() > 0)
  {
    char cmd = Serial.read();
    switch (cmd)
    {
      case 'R':
        int r;
        int g;
        int b;
        int c;
        for(int i = 0; i < 100; i++)
        {
          tselect(LEFT);
          tcs.getRawData(&r, &g, &b, &c);
          Serial.print(r);
          Serial.print(" ");
          Serial.print(g);
          Serial.print(" ");
          Serial.print(b);
          Serial.print(" ");
          Serial.print(c);
          Serial.print("\t");
          tselect(RIGHT);
          tcs.getRawData(&r, &g, &b, &c);
          Serial.print(r);
          Serial.print(" ");
          Serial.print(g);
          Serial.print(" ");
          Serial.print(b);
          Serial.print(" ");
          Serial.println(c);
          delay(100);
        }
        break;
      case 'Q':
        float ra;
        float ga;
        float ba;
        float ca;
        for(int i = 0; i < 100; i++)
        {
          tselect(LEFT);
          tcs.getRGB(&ra, &ga, &ba);
          Serial.print(ra);
          Serial.print(" ");
          Serial.print(ga);
          Serial.print(" ");
          Serial.print(ba);
          Serial.print("\t"); 
          tselect(RIGHT);
          tcs.getRGB(&ra, &ga, &ba);
          Serial.print(ra);
          Serial.print(" ");
          Serial.print(ga);
          Serial.print(" ");
          Serial.println(ba);
          delay(100);
        }
        break;
    }
  }
}
