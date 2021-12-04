#include <Arduino.h>

const double SONGDURATION = 15.9;  // Seconds
const uint8_t CARDPIN = D4;

void sleep(double seconds) {
  //ESP.deepSleep(seconds * 1000 * 1000, RF_DISABLED);
  delay(seconds * 1000);
}

void sing() {
  digitalWrite(CARDPIN, HIGH);  // Toggle transistor on -> bday card starts playing (or at least it SHOULD...)
  sleep(SONGDURATION);          // Wait for song to finish
  digitalWrite(D4, LOW);        // Toggle transistor off -> bday card shut up
}

void setup() {
  // Set PIN low (i.e. disable the bday card player) ASAP
  pinMode(CARDPIN, OUTPUT);
  digitalWrite(CARDPIN, LOW);
}

void loop() {
  sing();
  sleep(random(300));
}