#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <config.h>

const double SONGDURATION = 15.9;  // Seconds
const uint8_t CARDPIN = D4;
const unsigned long BIRTHDAY = 1639378800; // Monday, December 13, 2021 8:00:00 AM GMT+01:00
const unsigned long DEEPSLEEP = 300;  // Seconds

WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setupWiFi() {
  // Start WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  disconnectedEventHandler = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& event)
  {
    Serial.println("Disconnected!");
    WiFi.reconnect();
  });

  gotIpEventHandler = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& event)
  {
    Serial.printf("Got IP: %s\n", event.ip.toString().c_str());
  });

}

void startWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void sleep(double seconds) {
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
  pinMode(D0, WAKEUP_PULLUP);

  Serial.begin(460800);
  setupWiFi();
  startWiFi();

  timeClient.begin();
}

void loop() {
  timeClient.update();
  unsigned long currenttime = timeClient.getEpochTime();
  if (currenttime > BIRTHDAY) { 
    Serial.println("Yay!");
    sing();
    sleep(random(300));
  } else {
    Serial.printf("Nope. At least %lu seconds left. Snoozing for %lu seconds\n", BIRTHDAY - currenttime, DEEPSLEEP);
    sleep(DEEPSLEEP); // ESP.deepsleep won't work for some reason. Screw it, using regular sleep.
  }
}