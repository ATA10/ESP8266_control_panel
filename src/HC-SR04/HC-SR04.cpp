#include "HC-SR04.h"
#include <Arduino.h>

HCSR04::HCSR04(int trig, int echo, ESP8266WebServer &srv) : server(srv) {
    trigPin = trig;
    echoPin = echo;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // /distance endpoint'ini oluştur
    server.on("/distance", std::bind(&HCSR04::handleRequest, this));
}

float HCSR04::getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    long duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2;  // cm cinsinden mesafe hesapla
}

void HCSR04::handleRequest() {
    float distance = getDistance();
    String json = "{ \"distance\": " + String(distance, 2) + " }";
    server.send(200, "application/json", json);
}
