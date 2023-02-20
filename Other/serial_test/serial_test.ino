#include <avr/wdt.h>
void setup() {
  Serial2.begin(9600);
  Serial.begin(9600);
  Serial.println("reset");
  while (Serial2.available()) {
    uint8_t c = Serial2.read();
  }
  Serial.println("exit");
}

void resetFunc() {
  MCUSR = 0;
  wdt_enable(WDTO_250MS);
  while (1)
    ;
}

void loop() {
  if (Serial2.available()) {
    uint8_t c = Serial2.read();
    Serial.println(c);
    if (c == 132) resetFunc();
  }
  if (Serial.available()) {
    uint8_t num = 0;
    while (Serial.available()) {
      uint8_t c = Serial.read();
      num *= 10;
      num += c - '0';
      delay(100);
    }
    Serial.println(num);
    Serial2.write(num);
  }
}
