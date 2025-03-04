#ifndef WEBMANAGER_H
#define WEBMANAGER_H

#include <ESP8266WebServer.h>

class WebManager {
public:
    WebManager(ESP8266WebServer& server);
    void begin();
    void updateSensorData(float distance, int potValue);
    
private:
    ESP8266WebServer& _server;
    float sensorDistance;
    int potMeterValue;

    void handleRoot();
    void handleSensorData();
};

#endif
