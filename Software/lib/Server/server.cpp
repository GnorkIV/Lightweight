#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "server.h"
#include "WIFIinfo.h"

// -------------------------------------------Server Stuff
 
AsyncWebServer server(80);

void setupServer(){
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    }
    
    Serial.println(WiFi.localIP());

    server.on(
        "/post",
        HTTP_POST,
        [](AsyncWebServerRequest * request){},
        NULL,
        [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    
        String data_string = "";
        for (size_t i = 0; i < len; i++) {
            Serial.write(data[i]);
            data_string += (char)data[i];
        }
    
        Serial.println();

        Serial.println("REPEAT:"+ data_string);

    
        request->send(200);
    });
    
    server.begin();
}