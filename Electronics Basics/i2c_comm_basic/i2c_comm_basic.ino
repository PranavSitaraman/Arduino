#include<Wire.h>
int sevenbitaddress;
int memaddress = 0x49;
const int datasize = 6;
int values[datasize];
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  for (int i = 0; i < 128; i++)
  {
    Wire.beginTransmission(i);
    Wire.write(memaddress);
    int err = Wire.endTransmission();
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
void loop()
{
  Wire.beginTransmission(sevenbitaddress);
  Wire.write(memaddress);
  int err = Wire.endTransmission();
  if (err == 1)
  {
    Serial.println("Data too long");
  }
  if (err == 2 || err == 3)
  {
    Serial.println("NACK");
  }
  if (err == 4)
  {
    Serial.println("Other error");
  }
  int bytesreceived = Wire.requestFrom(sevenbitaddress, datasize);
  if (Wire.available() == bytesreceived)
  {
    for (int i = 0; i < bytesreceived; i++)
    {
      values[i] = Wire.read();
      Serial.print(values[i]);
      Serial.print(" ");
    }
    Serial.println();
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
