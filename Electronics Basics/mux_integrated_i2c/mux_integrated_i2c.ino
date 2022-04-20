#include<Wire.h>
#include<VL53L0X.h>
#include<Adafruit_BNO055.h>
#include <Adafruit_AS7341.h>
const int VLX[] = {1, 2, 3, 4};
const int RGB[] = {0};
const int MEL[] = {2, 3};
const int BOS[] = {5, 6};
int sevenbitaddress = 0x5A;
int ambientaddress = 0x06;
int objectaddress = 0x07;
int muxaddress = 0x70;
const int datasize = 3;
unsigned int ambient[datasize];
unsigned int object[datasize];
unsigned int gyro[6];
VL53L0X tof;
Adafruit_AS7341 color;
Adafruit_BNO055 bno = Adafruit_BNO055(55);
int tcaselect(uint8_t i)
{
  if (i > 7) return -4;
  Wire.beginTransmission(muxaddress);
  Wire.write(1 << i);
  return (Wire.endTransmission());
}
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  for (int port : BOS)
  {
    tcaselect(port);
    if(!bno.begin())
    {
      Serial.print("BNO failed");
      while (1) {}
    }
  }
  for (int port : VLX)
  {
    tcaselect(port);
    if (!tof.init())
    {
      Serial.print("Time of flight ");
      Serial.print(port);
      Serial.print(" failed");
      while (1) {}
    }
    tof.setTimeout(500);
    tof.startContinuous();
  }
  for (int port : RGB)
  {
    tcaselect(port);
    if (!color.begin())
    {
      Serial.print("Color sensor ");
      Serial.print(port);
      Serial.print(" failed");
      while (1) {}
    }
    color.setATIME(100);
    color.setASTEP(100);
    color.setGain(AS7341_GAIN_256X);
  }
}
void findtemp()
{
  for (int port : MEL)
  {
    tcaselect(port);
    Wire.beginTransmission(sevenbitaddress);
    Wire.write(ambientaddress);
    int err = Wire.endTransmission(false);
    if (err == 0)
    {
      int bytesreceived = Wire.requestFrom(sevenbitaddress, datasize);
      for (int i = 0; i < bytesreceived; i++)
      {
        ambient[i] = Wire.read();
      }
      float ambienttemp = ((ambient[1] << 8) | ambient[0]) * 0.02;
      Serial.print("Ambient: ");
      Serial.print(ambienttemp);
      Serial.print(" K, ");
      ambienttemp = ambienttemp - 273.15;
      Serial.print(ambienttemp);
      Serial.print(" C, ");
      ambienttemp = ambienttemp * 9/5 + 32;
      Serial.print(ambienttemp);
      Serial.print(" F");
      Serial.print("\t");
    }
    else
    {
      Serial.print("Temp failed");
      while (1) {}
    }
    Wire.beginTransmission(sevenbitaddress);
    Wire.write(objectaddress);
    err = Wire.endTransmission(false);
    if (err == 0)
    {
      int bytesreceived = Wire.requestFrom(sevenbitaddress, datasize);
      for (int i = 0; i < bytesreceived; i++)
      {
        object[i] = Wire.read();
      }
      float objecttemp = ((object[1] << 8) | object[0]) * 0.02;
      Serial.print("Object: ");
      Serial.print(objecttemp);
      Serial.print(" K, ");
      objecttemp = objecttemp - 273.15;
      Serial.print(objecttemp);
      Serial.print(" C, ");
      objecttemp = objecttemp * 9/5 + 32;
      Serial.print(objecttemp);
      Serial.print(" F");
      Serial.print("\t");
    }
    else
    {
      Serial.print("Temp failed");
      while (1) {}
    }
  }
}
void finddist()
{
  for (int port : VLX)
  {
    tcaselect(port);
    Serial.print("Port ");
    Serial.print(port);
    Serial.print(" - Dist: ");
    Serial.print(tof.readRangeContinuousMillimeters());
    Serial.print("\t");
  }
}
void findcolor()
{
  for (int port : RGB)
  {
    tcaselect(port);
    uint16_t readings[12];
    color.readAllChannels(readings);
    Serial.print("Port ");
    Serial.print(port);
    Serial.print(" - R: ");
    Serial.print(readings[8]);
    Serial.print(", G: ");
    Serial.print(readings[3]);
    Serial.print(", B: ");
    Serial.print(readings[2]);
    Serial.print("\t");
  }
}
void findgyro()
{
  for (int port : BOS)
  {
    tcaselect(port);
    Serial.print("Port ");
    Serial.print(port);
    sensors_event_t event;
    bno.getEvent(&event);
    Serial.print(" - X: ");
    Serial.print(event.orientation.x, 4);
    Serial.print(", Y: ");
    Serial.print(event.orientation.y, 4);
    Serial.print(", Z: ");
    Serial.print(event.orientation.z, 4);
    Serial.print("\t");
  }
}
void loop()
{
  //findtemp();
  findcolor();
  finddist();
  findgyro();
  Serial.println();
}
