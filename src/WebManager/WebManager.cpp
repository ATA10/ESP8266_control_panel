#include "WebManager.h"

const char HTML_PAGE[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
    <html lang="tr">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Röle Kontrol Paneli</title>
        <style>
            body { font-family: Arial, sans-serif; text-align: center; }
            button { font-size: 18px; margin: 10px; padding: 10px; }
        </style>
    </head>
    <body>
        <h2>Nodemcu Kontrol Paneli</h2>
    
        <h3>Mesafe Sensörü: <span id="distance">--</span> cm</h3>
        <h3>Potansiyometre: <span id="potentiometer">--</span></h3>
    
        <button onclick="controlRelay(1, 1)">Röle 1 Aç</button>
        <button onclick="controlRelay(1, 0)">Röle 1 Kapat</button>
        <br>
        <button onclick="controlRelay(2, 1)">Röle 2 Aç</button>
        <button onclick="controlRelay(2, 0)">Röle 2 Kapat</button>
        <br>
        <button onclick="controlRelay(3, 1)">Röle 3 Aç</button>
        <button onclick="controlRelay(3, 0)">Röle 3 Kapat</button>
    
        <script>
            function controlRelay(relay, state) {
                fetch(`/relay?relay=${relay}&state=${state}`)
                    .then(response => response.text())
                    .then(data => alert(data))
                    .catch(error => console.error("Hata:", error));
            }
    
            function updateSensorData() {
                fetch('/sensor')
                    .then(response => response.json())
                    .then(data => {
                        document.getElementById('distance').innerText = data.distance.toFixed(2);
                        document.getElementById('potentiometer').innerText = data.potentiometer;
                    })
                    .catch(error => console.error("Sensör verileri alınamadı:", error));
            }
    
            setInterval(updateSensorData, 1000); // Her saniyede bir güncelle
        </script>
    </body>
    </html>
    )rawliteral";
    

WebManager::WebManager(ESP8266WebServer& server) : _server(server), sensorDistance(0), potMeterValue(0) {}

void WebManager::begin() {
    _server.on("/", std::bind(&WebManager::handleRoot, this));
    _server.on("/sensor", std::bind(&WebManager::handleSensorData, this));
}

void WebManager::handleRoot() {
    _server.send(200, "text/html", HTML_PAGE);
}

void WebManager::updateSensorData(float distance, int potValue) {
    sensorDistance = distance;
    potMeterValue = potValue;
}

void WebManager::handleSensorData() {
    String json = "{";
    json += "\"distance\": " + String(sensorDistance, 2) + ",";
    json += "\"potentiometer\": " + String(potMeterValue);
    json += "}";

    _server.send(200, "application/json", json);
}
