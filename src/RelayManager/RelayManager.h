#ifndef RELAYMANAGER_H
#define RELAYMANAGER_H

#include <ESP8266WebServer.h>
#include "../Project_info_panel.h"
#include "../FirestoreManager/FirestoreManager.h"

class RelayManager {
public:
    RelayManager(ESP8266WebServer& server, int relay1, int relay2, int relay3, FirestoreManager& firestoreManager);
    void begin();
    void handle();
    int getRelayPin(int relayNumber);

private:
    ESP8266WebServer& _server;
    int _relay1, _relay2, _relay3;
    FirestoreManager& _firestoreManager;

    void handleRelayControl();
};

#endif
