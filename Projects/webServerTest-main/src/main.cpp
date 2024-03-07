#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
AsyncWebServer server(80);

typedef struct gasconfig{
    String devId = "gasId";
}GasConfig;

typedef struct wificonfig{ 
    String ssid = "ssid";
    String ps = "ps";
}WifiConfig;

typedef struct configdata{
    GasConfig gas;
    WifiConfig wifi;
}ConfigData;


ConfigData config;

void getData(String *data, AsyncWebServerRequest *request);


void setup() {
    Serial.begin(9600);

    // Start SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Set up Access Point
    WiFi.softAP("ESP32-Access-Point", "password");
    Serial.println(WiFi.softAPIP());
    
    // Web server
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    

    server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request){
        DynamicJsonDocument jsonDoc(1024);
        jsonDoc["ssid"] = config.wifi.ssid;
        jsonDoc["ps"] = config.wifi.ps;
        jsonDoc["gasId"] = config.gas.devId;
        // JSONオブジェクトを文字列に変換
        String jsonResponse;
        serializeJson(jsonDoc, jsonResponse);

        // レスポンスを送信
        request->send(200, "application/json", jsonResponse);
    });



    server.on("/ssidUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.print("ssid:");
        getData(&config.wifi.ssid, request);
    });

    server.on("/psUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.print("ps:");
        getData(&config.wifi.ps, request);
    });

    server.on("/gasIdUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.print("gasId:");
        getData(&config.gas.devId, request);
    });

    server.begin();
}

void getData(String *data, AsyncWebServerRequest *request){
    String newData = request->arg("newData");
    *data = newData;
    Serial.println(*data);
    request->send(200);
}

void loop() {
    // Additional loop tasks, if any
}
