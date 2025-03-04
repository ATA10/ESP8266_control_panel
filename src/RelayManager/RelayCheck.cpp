#include "RelayCheck.h"

RelayCheck::RelayCheck(FirestoreManager& firestoreManager, RelayManager& relayManager)
    : _firestoreManager(firestoreManager), _relayManager(relayManager) {}

void RelayCheck::updateRelays() {
    String jsonData = _firestoreManager.readData("devices", deviceID);

    if (jsonData.isEmpty()) {
        Serial.println("Firestore'dan veri alınamadı.");
        return;
    }

    bool relay1State = JsonParser::getRelayState(jsonData, "relay1");
    bool relay2State = JsonParser::getRelayState(jsonData, "relay2");
    bool relay3State = JsonParser::getRelayState(jsonData, "relay3");

    // Röle pinlerini RelayManager içinden alalım
    digitalWrite(_relayManager.getRelayPin(1), relay1State ? LOW : HIGH);
    digitalWrite(_relayManager.getRelayPin(2), relay2State ? LOW : HIGH);
    digitalWrite(_relayManager.getRelayPin(3), relay3State ? LOW : HIGH);

    Serial.println("Röleler güncellendi.");
}
