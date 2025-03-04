#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <ESP8266WebServer.h>

class Potentiometer {
public:
    Potentiometer(int pin, ESP8266WebServer &server);
    int readValue();
    void handleRequest();

private:
    int pin;
    ESP8266WebServer &server;
};

#endif
