#include <Arduino.h>

double songduration = 15.9;  // Seconds

void sleep(double seconds) {
  //ESP.deepSleep(seconds * 1000 * 1000, RF_DISABLED);
  delay(seconds * 1000);
}

void sing() {
  digitalWrite(D4, HIGH); 
  sleep(songduration);
  digitalWrite(D4, LOW); 
}

void setup() {
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);
  delay(1000);
}

void loop() {
  sing();
  sleep(random(300));
}
