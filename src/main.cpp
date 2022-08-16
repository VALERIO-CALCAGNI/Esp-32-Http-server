////////////////////////////////// LIBRARIES
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ArduinoJson.h>

#include "communication/communication.h"
#include "global/global.h"

StaticJsonDocument<200> doc;

////////////////////////////////// SETUP
void setup() 
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  
  connectToWiFi();
}


////////////////////////////////// LOOP
void loop() 
{
   WiFiClient client =  getClient();

  if(client){
    String currentLine = ""; 

    while(client.connected()){
      if(client.available()){
        char c = client.read();

        if(c == '\n'){
          if (currentLine.length() == 0) {
            serializeJson(doc, Serial); // print in serial monitor

            if(doc["method"] == "POST"  && doc["url"] == "/led"){
              digitalWrite(LED, doc["state"]);
              client.println("HTTP/1.1 200 OK");
            }
            else if(doc["method"] == "GET"  && doc["url"] == "/led"){
              client.println("HTTP/1.1 200 OK");
              client.print("state: ");
              client.println(digitalRead(LED));
            }

            client.println("Content-type: text/html");
            client.println("Connection: close");
            doc.clear();
            break;
          }else{
            parseRequestLine(currentLine, &doc);                    
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;     
        }
      }
    }

    client.stop();
    Serial.println("client disconnected");
  }
}