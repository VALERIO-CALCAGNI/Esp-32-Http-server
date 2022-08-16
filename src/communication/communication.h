#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#ifndef communication_h
#define communication_h

void connectToWiFi();
void parseRequestLine(String currentLine, StaticJsonDocument<200> *doc);
WiFiClient getClient();

#endif