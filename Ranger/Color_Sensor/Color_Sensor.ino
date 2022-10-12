#include <Arduino.h>
#include <MeAuriga.h>
#include <Wire.h>
int colorPin = 1;
MeColorSensor colorsensor(colorPin);
void setup() {
  Wire.begin();
  Serial.begin(9600);  // for serial monitor display
  colorsensor.SensorInit();
  colorsensor.TurnOnLight();
}
void loop() {
  long rgb = 0;
  colorsensor.ColorDataRead();
  rgb = colorsensor.ReturnColorCode();
  Serial.print("R: ");
  Serial.println((uint8_t)(rgb >> 16) & 0x0000ff);
  Serial.print("G: ");
  Serial.println((uint8_t)(rgb >> 8) & 0x0000ff);
  Serial.print("B: ");
  Serial.println((uint8_t)(rgb & 0x0000ff));
  Serial.print("GrayScale: ");
  Serial.println(colorsensor.ReturnGrayscale());
  delay(1000);
}