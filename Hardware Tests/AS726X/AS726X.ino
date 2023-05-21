#include <Adafruit_BNO055.h>
#include <Arduino.h>
#include <MeMegaPi.h>
#include <Adafruit_AS726x.h>
#include <VL53L0X.h>
#include <Stepper.h>
#include <Wire.h>
#include <avr/wdt.h>
struct Color
{
  float R;
  float G;
  float B;
};
Adafruit_AS726x color[2];
constexpr uint8_t COLOR[]{2, 3};
void tcaselect(uint16_t i)
{
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  Wire.endTransmission();
}
Color tiles(uint16_t port)
{
  float sensorValues[AS726x_NUM_CHANNELS];
  tcaselect(COLOR[port]);
  while (!color[port].dataReady());
  color[port].readCalibratedValues(sensorValues);
  Color colors = {sensorValues[AS726x_RED], sensorValues[AS726x_GREEN], sensorValues[AS726x_BLUE]};
  return colors;
}
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  for (int i = 0; i < sizeof(COLOR)/sizeof(*COLOR); i++)
  {
    tcaselect(COLOR[i]);
    color[i].begin();
    color[i].setConversionType(MODE_2);
    color[i].drvOn();
  }
}
void loop()
{
  Color color = tiles(0);
  Serial.print(color.R);
  Serial.print("\t");
  Serial.print(color.G);
  Serial.print("\t");
  Serial.print(color.B);
  Serial.print("\t\t");
  color = tiles(1);
  Serial.print(color.R);
  Serial.print("\t");
  Serial.print(color.G);
  Serial.print("\t");
  Serial.print(color.B);
  Serial.println();
}