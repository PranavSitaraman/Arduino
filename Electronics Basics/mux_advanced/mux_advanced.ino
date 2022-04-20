#include <Wire.h>
#include <VL53L0X.h>
#include <Adafruit_TCS34725.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_MLX90614.h>
const int VLX[] = {6, 7, 4};
const int RGB[] = {5};
const int MLX[] = {2, 3};
const int BOS[] = {1};
int muxaddress = 0x70;
VL53L0X tof;
Adafruit_TCS34725 color(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_4X);
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
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
  for (int port : MLX)
  {
    tcaselect(port);
    if (!mlx.begin())
    {
      Serial.print("MLX failed");
      while (1) {}
    }
  }
  for (int port : BOS)
  {
    tcaselect(port);
    if (!bno.begin(Adafruit_BNO055::OPERATION_MODE_IMUPLUS))
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
      Serial.print("TOF failed");
      Serial.print(port);
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
      Serial.print("RGB failed");
      Serial.print(port);
      while (1) {}
    }
  }
}
void findtemp()
{
  for (int port : MLX)
  {
    tcaselect(port);
    Serial.print("Ambient: ");
    Serial.print(mlx.readAmbientTempF());
    Serial.print(", Object: ");
    Serial.print(mlx.readObjectTempF());
    Serial.print("\t\t");
  }
}
void finddist()
{
  for (int port : VLX)
  {
    tcaselect(port);
    Serial.print("Dist: ");
    Serial.print(tof.readRangeContinuousMillimeters());
    Serial.print("\t\t");
  }
}
void findcolor()
{
  for (int port : RGB)
  {
    tcaselect(port);
    uint16_t r, g, b, c;
    color.getRawData(&r, &g, &b, &c);
    Serial.print("R: ");
    Serial.print(r);
    Serial.print(", G: ");
    Serial.print(g);
    Serial.print(", B: ");
    Serial.print(b);
    Serial.print(", C: ");
    Serial.print(c);
    Serial.print("\t\t");
  }
}
void findgyro()
{
  for (int port : BOS)
  {
    tcaselect(port);
    sensors_event_t event;
    bno.getEvent(&event);
    Serial.print("X: ");
    Serial.print(event.orientation.x, 4);
    Serial.print(", Y: ");
    Serial.print(event.orientation.y, 4);
    Serial.print(", Z: ");
    Serial.print(event.orientation.z, 4);
    Serial.print("\t\t");
  }
}
void loop()
{
  findtemp();
  finddist();
  findcolor();
  findgyro();
  Serial.println();
}
