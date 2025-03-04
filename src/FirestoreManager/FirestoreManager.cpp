#include "FirestoreManager.h"
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>
#include <HTTPClient.h>

FirestoreManager::FirestoreManager(const String& projectId, const String& apiKey) {
    this->firebaseHost = "https://firestore.googleapis.com/v1/projects/" + projectId + "/databases/(default)/documents";
    this->apiKey = apiKey;
}

void FirestoreManager::writeDataSensor(const String& collection, const String& document, float temperature, int humidity) {
    if (WiFi.status() == WL_CONNECTED) {
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setInsecure();

        HTTPClient http;

        // ✅ Firestore URL (Sadece "sensors" alanı güncellenecek)
        String url = "https://firestore.googleapis.com/v1/projects/" + projectId + "/databases/(default)/documents/" 
                     + collection + "/" + document + "?updateMask.fieldPaths=sensors&key=" + apiKey;

        http.begin(*client, url);
        http.addHeader("Content-Type", "application/json");

        // ✅ Yeni ArduinoJson standardına uygun JSON oluşturma
        DynamicJsonDocument jsonDoc(256);
        JsonObject fields = jsonDoc["fields"].to<JsonObject>();  
        JsonObject sensors = fields["sensors"].to<JsonObject>(); 
        JsonObject mapValue = sensors["mapValue"].to<JsonObject>();  
        JsonObject sensorFields = mapValue["fields"].to<JsonObject>();  

        // Sensör değerlerini JSON'a ekleme
        sensorFields["humidity"]["stringValue"] = String(humidity);
        sensorFields["temperature"]["stringValue"] = String(temperature);

        String jsonString;
        serializeJson(jsonDoc, jsonString);

        Serial.println("📤 Firestore'a gönderilecek JSON:");
        Serial.println(jsonString);

        // ✅ `PATCH` ile sadece "sensors" güncelleniyor
        int httpResponseCode = http.PATCH(jsonString);

        if (httpResponseCode > 0) {
            Serial.println("✅ Sensör verileri Firestore'a yazıldı!");
            Serial.println(http.getString());
        } else {
            Serial.print("❌ HTTP Hatası: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("❌ WiFi bağlantısı yok!");
    }
}


void FirestoreManager::writeDataRelay(const String& collection, const String& document, const String& relayName, bool relayState) {
    if (WiFi.status() == WL_CONNECTED) {
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setInsecure();

        HTTPClient http;

        // ✅ Sadece değişen röleyi güncellemek için Firestore URL'si
        String url = "https://firestore.googleapis.com/v1/projects/" + projectId + "/databases/(default)/documents/" 
                     + collection + "/" + document + "?updateMask.fieldPaths=relays." + relayName + "&key=" + apiKey;

        http.begin(*client, url);
        http.addHeader("Content-Type", "application/json");

        // 🔥 Güncellenmiş JSON kullanımı
        DynamicJsonDocument jsonDoc(256); // ✅ Eski StaticJsonDocument yerine DynamicJsonDocument
        JsonObject fields = jsonDoc["fields"].to<JsonObject>();  // ✅ Yeni nesil Json oluşturma yöntemi
        JsonObject relays = fields["relays"].to<JsonObject>();
        JsonObject mapValue = relays["mapValue"].to<JsonObject>();
        JsonObject relayFields = mapValue["fields"].to<JsonObject>();

        relayFields[relayName]["booleanValue"] = relayState;  // 🔥 Değişen röle değerini ekle

        String jsonString;
        serializeJson(jsonDoc, jsonString);

        Serial.println("📤 Firestore'a gönderilecek JSON:");
        Serial.println(jsonString);

        // ✅ `PATCH` kullanarak sadece değişen röleyi güncelliyoruz
        int httpResponseCode = http.PATCH(jsonString);

        if (httpResponseCode > 0) {
            Serial.println("✅ Röle verisi Firestore'a yazıldı!");
            Serial.println(http.getString());
        } else {
            Serial.print("❌ HTTP Hatası: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("❌ WiFi bağlantısı yok!");
    }
}


String FirestoreManager::readData(const String& collection, const String& document) {
    if (WiFi.status() == WL_CONNECTED) {
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setInsecure();

        HTTPClient http;
        String url = firebaseHost + "/" + collection + "/" + document + "?key=" + apiKey;

        http.begin(*client, url);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            String payload = http.getString();
            Serial.println("Firestore'dan gelen veri:");
            Serial.println(payload);
            http.end();
            return payload;
        } else {
            Serial.print("❌ HTTP Hatası: ");
            Serial.println(httpResponseCode);
            http.end();
            return "";
        }
    }
    return "";
}

