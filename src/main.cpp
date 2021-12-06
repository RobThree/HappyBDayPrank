#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <config.h>

const char* HOST = HOME;

void sing() {
  digitalWrite(CARD_PIN, HIGH);  // Toggle transistor on -> bday card starts playing (or at least it SHOULD...)
  delay(SONG_DURATION * 1e3);    // Wait for song to finish
  digitalWrite(CARD_PIN, LOW);   // Toggle transistor off -> bday card shut up
}

void setup() {
  unsigned long deepsleep = SLEEP_POLL_INTERVAL * 1e6;  // Default deep sleep

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
    while (WiFi.status() != WL_CONNECTED) // Wait until WiFi is connected
      delay(500);

    // Get time
    Serial.println("What time is it?");
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, NTP_SERVER, NTP_TIME_OFFSET);
    timeClient.begin();
    timeClient.update();
    unsigned long currenttime = timeClient.getEpochTime();

    bool happybirthday = currenttime > BIRTHDAY_TIMESTAMP;

    // Call home
    char uri[100];
    char path[50];
    sprintf(path, PATH, currenttime, happybirthday);
    sprintf(uri, "https://%s%s", HOME, path);

    Serial.printf("Calling home: %s\n", uri);

    WiFiClientSecure client;
    HTTPClient http;
    client.connect(HOST, SSL_PORT);
    client.setInsecure();
    http.begin(client, uri);
    http.GET();

    // Is it time yet?
    Serial.println("Time to sing?");
    if (happybirthday)
    {
      Serial.println("Yay! Happy birthday!");
      sing();
      deepsleep = (SONG_INTERVAL_MIN + random(SONG_INTERVAL_MAX - SONG_INTERVAL_MIN)) * 1e6;  // Sleep random amount
    } else {
      Serial.printf("Nope! Not yet! Wait at least %lu more seconds\n", BIRTHDAY_TIMESTAMP - currenttime);
    }

    // Prepare for deep sleep
    Serial.println("Getting ready for bed... yawn *stretches*");

    // Stop timeclient
    timeClient.end();

    // Disconnect WiFi
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);

    Serial.println("Goodnight!");
    ESP.deepSleep(deepsleep);
  }
}

void loop() { }