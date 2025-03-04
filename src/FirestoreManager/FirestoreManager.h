#ifndef FIRESTOREMANAGER_H
#define FIRESTOREMANAGER_H

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ESP8266HTTPClient.h> // ESP8266 için HTTPClient
#include <WiFiClientSecureBearSSL.h> // HTTPS bağlantıları için
#include "JsonParser/JsonParser.h"

class FirestoreManager {
public:
    FirestoreManager(const String& projectId, const String& apiKey);
    void writeDataSensor(const String& collection, const String& document, float temperature, int humidity);
    void writeDataRelay(const String& collection, const String& document, const String& relayName, bool relayState);
    String readData(const String& collection, const String& document);

private:
    String firebaseHost;
    String apiKey;
};

#endif

