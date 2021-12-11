#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <config.h>

const char* HOST = HOME;
const String happy = String("yay");

void sing() {
  digitalWrite(CARD_PIN, HIGH);  // Toggle transistor on -> bday card starts playing (or at least it SHOULD...)
  delay(SONG_DURATION * 1e3);    // Wait for song to finish
  digitalWrite(CARD_PIN, LOW);   // Toggle transistor off -> bday card shut up
}

void setup() {
  unsigned long deepsleep = SLEEP_POLL_INTERVAL;  // Default deep sleep
  int retry = 0;

  // Set PIN low (i.e. disable the bday card player) ASAP
  pinMode(CARD_PIN, OUTPUT);
  digitalWrite(CARD_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(D0, WAKEUP_PULLUP);
  Serial.begin(SERIAL_SPEED);

  while (true) {
    Serial.println("Starting WiFi");
    // Start WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    while (WiFi.status() != WL_CONNECTED && retry++ < RETRYCOUNT) { // Wait until WiFi is connected or retries exceeded
      delay(1000);
      Serial.printf("WiFi status: %d\n", WiFi.status());
    }

    if (WiFi.status() == WL_CONNECTED) {
      // Read battery voltage
      float batt = (float)analogRead(A0) * 0.00449;
      Serial.printf("Voltage: %f\n", batt);

      // Call home
      char uri[100];
      char path[50];
      sprintf(path, PATH, batt);
      sprintf(uri, "https://%s%s", HOME, path);

      Serial.printf("Calling home: %s\n", uri);

      WiFiClientSecure client;
      HTTPClient http;
      client.connect(HOST, SSL_PORT);
      client.setInsecure();
      http.begin(client, uri);
      http.GET();

      String payload = http.getString();

      // Is it time yet?
      Serial.println(payload);
      if (payload.equals(happy))
      {
        sing();
        deepsleep = (SONG_INTERVAL_MIN + random(SONG_INTERVAL_MAX - SONG_INTERVAL_MIN));  // Sleep random amount
      }
    }

    // Prepare for deep sleep
    Serial.println("Getting ready for bed... yawn *stretches*");

    // Disconnect WiFi
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);

    Serial.printf("Goodnight! Sleeping for %lu seconds\n", deepsleep);
    ESP.deepSleep(deepsleep * 1e6);
  }
}

void loop() { }