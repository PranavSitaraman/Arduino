#include <Arduino.h>
#include <MeAuriga.h>
MeRGBLed led(0, 12);      // initialize all 12 pixel leds
const int pixiePin = 44;  // the pin # the pixie ring is soldered on
int count = 0;
void setup() {
  led.setpin(pixiePin);  // create a handle to the led pin
}
void loop() {
  led.setColor(count % 12, 0, 0, 0);
  led.setColor(count % 12 + 1, 255, 0, 0);
  led.show();
  delay(2000);
  ++count;
}