// COPY THIS FILE AND RENAME IT TO "config.h" AND ENTER ALL YOUR CONFIGURATION DATA BELOW

#ifndef CONFIG_H
#define CONFIG_H

// Replace with your actual SSID and password:
#define WIFI_SSID           "***SSID***"
#define WIFI_PASSWD         "***WIFIPASSWORD***"

#define SERIAL_SPEED        460800
#define CARD_PIN            D1
#define SSL_PORT            443

// Play song interval (in seconds)
#define SONG_INTERVAL_MIN   30
#define SONG_INTERVAL_MAX   300
// Other intervals (in seconds)
#define SONG_DURATION       15.9
#define SLEEP_POLL_INTERVAL 300

#define HOME                "foo.bar.com"
#define PATH                "/bday/"
#endif