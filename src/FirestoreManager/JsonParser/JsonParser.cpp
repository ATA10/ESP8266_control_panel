#include "JsonParser.h"

bool JsonParser::getRelayState(const String& json, const String& relayName) {
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error) {
        Serial.print("❌ JSON Ayrıştırma Hatası: ");
        Serial.println(error.f_str());
        return false; // Varsayılan olarak kapalı dönebiliriz
    }

    Serial.println("✅ JSON Ayrıştırma Başarılı!");
    
    // JSON'u ekrana yazdırarak doğrulayalım
    Serial.println("📜 Firestore JSON:");
    serializeJsonPretty(doc, Serial);
    Serial.println();

    // JSON'dan röle bilgilerine ulaşma
    JsonObject relays = doc["fields"]["relays"]["mapValue"]["fields"];
    
    if (!relays.containsKey(relayName)) {
        Serial.println("⚠️ Belirtilen röle bulunamadı!");
        return false;
    }

    // Boolean değeri oku
    bool state = relays[relayName]["booleanValue"];
    Serial.print("🔍 "); Serial.print(relayName); Serial.print(" Durumu: ");
    Serial.println(state ? "ON" : "OFF");

    return state;
}
