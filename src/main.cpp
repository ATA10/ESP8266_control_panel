#include "WiFiManager/WiFiManager.h"
#include "RelayManager/RelayManager.h"
#include "WebManager/WebManager.h"
#include "HC-SR04/HC-SR04.h"
#include "PotentiometerManager/Potentiometer.h"
#include "FirestoreManager/FirestoreManager.h"
#include "RelayManager/RelayCheck.h"
#include "Project_info_panel.h"

// #define trigPin D5  
// #define echoPin D6  

FirestoreManager firestore(projectId, apiKey);

extern RelayManager relayManager;

#define RELAY1 D2
#define RELAY2 D3
#define RELAY3 D4

RelayManager relayManager(server, RELAY1, RELAY2, RELAY3, firestore);
WebManager webManager(server);

HCSR04 sensor(D5, D6, server);
Potentiometer pot(A0, server);  

RelayCheck relayCheck(firestore, relayManager);

void setup() {
    Serial.begin(115200);

    checkResetButton();
    readWiFiCredentials();

    if (!connectToWiFi()) {
        Serial.println("⚠ Wi-Fi'ye bağlanılamadı, AP Modu başlatıldı.");
    } else {
        Serial.println("Nodemcu aktif ve çalışmaya hazır!!!");
    }

    webManager.begin();
    relayManager.begin();
    server.begin();
}

float lastDistance = -1.0;
int lastPotValue = -1;

void loop() {
    server.handleClient();
    checkResetButton();  

    relayManager.handle();
    relayCheck.updateRelays();

    // pot ve hc-sr04 veri okuma 
    float distance = sensor.getDistance();
    int potValue = pot.readValue();

    if (distance != lastDistance || potValue != lastPotValue) {
        Serial.println("🔄 Değer değişti, Firestore'a yazılıyor...");
        
        webManager.updateSensorData(distance, potValue);
        firestore.writeDataSensor("devices", "NuCntIHjaq8EnAsOShff", distance, potValue);

        // Yeni değerleri önceki değer olarak kaydet
        lastDistance = distance;
        lastPotValue = potValue;
    }

    delay(500);
}