#include "Potentiometer.h"
#include <Arduino.h>

Potentiometer::Potentiometer(int p, ESP8266WebServer &srv) : server(srv) {
    pin = p;

    // /potentiometer endpoint'ini oluştur
    server.on("/potentiometer", std::bind(&Potentiometer::handleRequest, this));
}

int Potentiometer::readValue() {
    return analogRead(pin);  // 0 - 1023 arası değer döndür
}

void Potentiometer::handleRequest() {
    int potValue = readValue();
    String json = "{ \"potentiometer\": " + String(potValue) + " }";
    server.send(200, "application/json", json);
}
