#include<Wire.h>
#include <Adafruit_TCS34725.h>
int sevenbitaddress = 0x5A;
int ambientaddress = 0x06;
int objectaddress = 0x07;
const int datasize = 3;
unsigned int ambient[datasize];
unsigned int object[datasize];
Adafruit_TCS34725 color(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Serial.println("Start");
  delay(100);
  for (int i = 0; i < 128; i++)
  {
    Wire.beginTransmission(i);
    Wire.write(ambientaddress);
    int err = Wire.endTransmission(false);
    Serial.print("Memory address: ");
    Serial.print(i);
    if (err == 0)
    {
      Serial.println(" - Address found");
      sevenbitaddress = i;
    }
    else
    {
      Serial.println(" - Address not found");
    }
  }
}
void findcolor()
{
  uint16_t r, g, b, c;
  color.getRawData(&r, &g, &b, &c);
  Serial.print("R: ");
  Serial.print(r);
  Serial.print("\t\tG: ");
  Serial.print(g);
  Serial.print("\t\tB: ");
  Serial.print(b);
}
void loop()
{
  findcolor();
  Serial.println();
}
