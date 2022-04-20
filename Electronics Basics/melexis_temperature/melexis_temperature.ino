#include<Wire.h>
int sevenbitaddress = 0x5A;
int ambientaddress = 0x06;
int objectaddress = 0x07;
const int datasize = 3;
unsigned int ambient[datasize];
unsigned int object[datasize];
void setup()
{
  Wire.begin();
  Serial.begin(9600);
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
void findambient()
{
  Wire.beginTransmission(sevenbitaddress);
  Wire.write(ambientaddress);
  int err = Wire.endTransmission(false);
  if (err == 0)
  {
    int bytesreceived = Wire.requestFrom(sevenbitaddress, datasize);
    if (Wire.available() == bytesreceived)
    {
      for (int i = 0; i < bytesreceived; i++)
      {
        ambient[i] = Wire.read();
      }
      float ambienttemp = ((ambient[1] << 8) | ambient[0]) * 0.02;
      Serial.print("Ambient: ");
      Serial.print(ambienttemp);
      Serial.print(" Kelvin, ");
      ambienttemp = ambienttemp - 273.15;
      Serial.print(ambienttemp);
      Serial.print(" Celsius, ");
      ambienttemp = ambienttemp * 9/5 + 32;
      Serial.print(ambienttemp);
      Serial.print(" Fahrenheit");
      Serial.print("\t\t");
    }
    else
    {
      Serial.print("Bytes received: ");
      Serial.println(bytesreceived);
      Serial.print("Bytes in buffer: ");
      Serial.println(Wire.available());
      while(Wire.available())
      {
        Wire.read();
      }
    }
  }
  else
  {
    Serial.println("Transmission Error");
  }
}
void findobject()
{
  Wire.beginTransmission(sevenbitaddress);
  Wire.write(objectaddress);
  int err = Wire.endTransmission(false);
  if (err == 0)
  {
    int bytesreceived = Wire.requestFrom(sevenbitaddress, datasize);
    if (Wire.available() == bytesreceived)
    {
      for (int i = 0; i < bytesreceived; i++)
      {
        object[i] = Wire.read();
      }
      float objecttemp = ((object[1] << 8) | object[0]) * 0.02;
      Serial.print("Object: ");
      Serial.print(objecttemp);
      Serial.print(" Kelvin, ");
      objecttemp = objecttemp - 273.15;
      Serial.print(objecttemp);
      Serial.print(" Celsius, ");
      objecttemp = objecttemp * 9/5 + 32;
      Serial.print(objecttemp);
      Serial.print(" Fahrenheit");
      Serial.print("\t\t");
    }
    else
    {
      Serial.print("Bytes received: ");
      Serial.println(bytesreceived);
      Serial.print("Bytes in buffer: ");
      Serial.println(Wire.available());
      while(Wire.available())
      {
        Wire.read();
      }
    }
  }
  else
  {
    Serial.println("Transmission Error");
  }
}
void loop()
{
  findambient();
  findobject();
  Serial.println();
}
