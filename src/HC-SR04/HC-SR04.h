#ifndef HCSR04_H
#define HCSR04_H

#include <ESP8266WebServer.h>

class HCSR04 {
public:
    HCSR04(int trigPin, int echoPin, ESP8266WebServer &server);
    float getDistance();
    void handleRequest();

private:
    int trigPin, echoPin;
    ESP8266WebServer &server;
};

#endif
