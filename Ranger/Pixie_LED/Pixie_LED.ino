#include <Arduino.h>
#include <MeAuriga.h>
#include <MeRGBLed.h>
MeRGBLed led(0, 12);      // initialize all 12 pixel leds to 0
const int pixiePin = 44;  // the pin # the pixie ring is soldered on
void setup() {
  led.setpin(pixiePin);  // create a handle to the led pin
}
void loop() {                  // red green blue
  led.setColor(1, 255, 0, 0);  // set the led#1 on to red (0 to 255)
  led.show();                  // must call this to turn on the led
  delay(1000);
  led.setColor(1, 0, 0, 0);  // turn off the led#1
  led.show();
  delay(1000);
}