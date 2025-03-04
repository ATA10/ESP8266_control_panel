#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define EEPROM_SIZE 96
#define RESET_BUTTON D1  

extern ESP8266WebServer server;
extern String wifi_ssid;
extern String wifi_password;

void saveWiFiCredentials(String ssid, String password);
void readWiFiCredentials();
bool connectToWiFi();
void startAPMode();
void handleWiFiPortal();
void checkResetButton();

// HTML Formu
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }
        form { max-width: 300px; margin: auto; padding: 20px; background: #f4f4f4; border-radius: 10px; }
        input { width: 100%; padding: 10px; margin: 10px 0; border-radius: 5px; border: 1px solid #ccc; }
        button { background: #28a745; color: white; border: none; padding: 10px; width: 100%; border-radius: 5px; }
    </style>
</head>
<body>
    <h2>Wi-Fi Bağlantı Formu</h2>
    <form action="/submit" method="GET">
        <label for="name">Wi-Fi Adı (SSID):</label>
        <input type="text" id="name" name="name" required>
        
        <label for="password">Wi-Fi Şifre:</label>
        <input type="text" id="password" name="password" required>
        
        <button type="submit">Gönder</button>
    </form>
</body>
</html>
)rawliteral";

#endif
