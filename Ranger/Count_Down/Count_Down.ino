#include <Arduino.h>  // make sure you include these header files
#include <MeAuriga.h>
int Count = 10;  // must know scope rule, global vs. local
void setup() {
  Serial.begin(9600);  //MUST be in the setup() in order to view console output
}
void loop() {
  Serial.print(“count
               : ”);  // show the output
  Serial.println(Count);
  Count = Count - 1;  // must know lvalue rule
  delay(1000);
  if (Count <= 0) {
    Count = 10;
    delay(3000);
    Serial.println(Count);
    Serial.println(“== == == == == == == =”);
  }
}