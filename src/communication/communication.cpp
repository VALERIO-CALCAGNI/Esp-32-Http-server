#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>

// TODO: resolve library import error

#include "communication.h"
#include "global/global.h"


WiFiServer server(80);
WiFiClient client;

// Set wifi configuration manually (optional)
IPAddress local_IP(192, 168, 100, 104);
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);


WiFiClient getClient()
{
  client = server.available();
  return client;
}

void connectToWiFi() 
{

  Serial.print("WiFi configuration...");

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure static address");
  }

  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA); // client mode 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.print("\nConnected at IP: ");
  Serial.println(WiFi.localIP()); // print esp32 IP

  if(MDNS.begin("esp32")){
    Serial.println("enabled dns: http://esp32.local/"); // print esp32 IP
  }

  server.begin();
}

void parseRequestLine(String currentLine, StaticJsonDocument<200> *doc) // parse a single line and attach key|value on json doc
{
  if(currentLine.indexOf("HTTP") != -1){
    // that's a different parse with first line {"method": "POST", "url" : "/led/on"}           
    String method = currentLine.substring(0, currentLine.indexOf(" "));
    String url = currentLine.substring(currentLine.indexOf(" "), currentLine.indexOf("HTTP"));
    method.trim();
    url.trim();
    (*doc)["method"] = method;
    (*doc)["url"] = url;
  }else{
    String key = currentLine.substring(0, currentLine.indexOf(" "));
    String value = currentLine.substring(currentLine.indexOf(" "), currentLine.length());
    key.replace(":","");
    key.trim();
    value.trim();
    (*doc)[key] = value;
  }
}