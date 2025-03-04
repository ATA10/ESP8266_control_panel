#include "RelayManager.h"

RelayManager::RelayManager(ESP8266WebServer& server, int relay1, int relay2, int relay3, FirestoreManager& firestoreManager)
    : _server(server), _relay1(relay1), _relay2(relay2), _relay3(relay3), _firestoreManager(firestoreManager) {}

void RelayManager::begin() {
    pinMode(_relay1, OUTPUT);
    pinMode(_relay2, OUTPUT);
    pinMode(_relay3, OUTPUT);
    digitalWrite(_relay1, HIGH);
    digitalWrite(_relay2, HIGH);
    digitalWrite(_relay3, HIGH);

    _server.on("/relay", std::bind(&RelayManager::handleRelayControl, this));
}

void RelayManager::handleRelayControl() {
    if (!_server.hasArg("relay") || !_server.hasArg("state")) {
        _server.send(400, "text/plain", "Hata: relay ve state parametreleri gerekli");
        return;
    }

    int relay = _server.arg("relay").toInt();
    int state = _server.arg("state").toInt();

    if (relay < 1 || relay > 3 || (state != 0 && state != 1)) {
        _server.send(400, "text/plain", "Hata: Geçersiz parametre");
        return;
    }

    int relayPin = (relay == 1) ? _relay1 : (relay == 2) ? _relay2 : _relay3;
    
    // // Röle durumu tersine çevirme
    // state = (state == 1) ? 0 : 1;
    // digitalWrite(relayPin, state);

    // Firestore'a röle durumu yazma
    String relayName = "relay" + String(relay);  // relay1, relay2, relay3
    _firestoreManager.writeDataRelay("devices", deviceID, relayName, state); // Firestore'a yazma


    _server.send(200, "text/plain", "Röle durumu güncellendi");
}

void RelayManager::handle() {
    _server.handleClient();
}

int RelayManager::getRelayPin(int relayNumber) {
    switch (relayNumber) {
        case 1: return _relay1;
        case 2: return _relay2;
        case 3: return _relay3;
        default: return -1;  // Geçersiz giriş
    }
}
