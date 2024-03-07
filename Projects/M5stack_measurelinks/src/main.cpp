// #include <Arduino.h>
// #include <M5Stack.h>
// #include <measure.h>
// // put function declarations here:
// void setup(){
//   Serial.begin(115200);
//   M5.begin();

//   M5.Lcd.setCursor(20,60);
//   M5.Lcd.setTextSize(4);
//   M5.Lcd.print("Hello world");
//   delay(1000);
//   initMeasure();
//   M5.Lcd.clear(BLACK);
// }

// void loop(){
//   char data[50];
//   sprintf(data, "%d", average(10));
//   M5.Lcd.setCursor(20,60);
//   M5.Lcd.clear(BLACK);
//   M5.Lcd.print(data);
//   delay(1000);
// }
#include <Arduino.h>
#include <M5Stack.h>
#include <M5GFX.h>
#include <HTTPClient.h>
// #include <HTS221.h>
#include "HX711.h"

M5GFX display;
M5Canvas canvas(&display);

// HX711 related pin Settings.  HX711 相关引脚设置
#define LOADCELL_DOUT_PIN 36
#define LOADCELL_SCK_PIN  26

#define SCALE (12.22f)

// Connecting WiFi Settings
const char* SSID = "ssid";           // Wi-Fi SSID
const char* PASSWORD = "password";   // Password
// Accessed Google Script Settings
const char* APP_SERVER = "script.google.com";
const char* KEY = "google_scripts_key";

HX711 scale;

void accessToGoogleSheets(int weight);

void setup() {
    M5.begin();        // Init M5Stack.  初始化M5Stack
    M5.Power.begin();  // Init power  初始化电源模块
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.createSprite(display.width(), display.height());
    canvas.setTextDatum(MC_DATUM);
    canvas.setPaletteColor(1, WHITE);

    canvas.drawString("Calibration sensor....", 160, 80);
    canvas.pushSprite(0, 0);
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN, 64);
    // The scale value is the adc value corresponding to 1g
    scale.set_scale(SCALE);  // set scale
    scale.tare();            // auto set offset
    
    WiFi.begin(SSID, PASSWORD);
    Serial.print("WiFi connecting");
    // Wait until succeed connecting.
    // If the number of checks is more than CHECK_NUM_MAX, give up connecting and
    // start deepsleep to prevent Joule heat from affecting next measurements.
    while (WiFi.status() != WL_CONNECTED) {
        // if(statusCheckCounter > CHECK_NUM_MAX) {
        //     WiFi.disconnect();
        //     Serial.println("failed");
        //     Serial.println("Deepsleep Start");
        //     espDeepSleep();
        // }
        Serial.print(WiFi.status());
        delay(500);
        // statusCheckCounter++;
    }
    Serial.println("\nconnected");

}

void loop() {
    canvas.fillSprite(BLACK);
    int weight = scale.get_units(10);
    canvas.setTextSize(5);
    canvas.drawString(String(weight) + "g", 160, 150);
    M5.update();
    if (M5.BtnA.wasPressed()) {
        scale.set_scale(SCALE);
        scale.tare();
    }
    canvas.pushSprite(0, 0);

    if (M5.BtnA.wasPressed()) {
        accessToGoogleSheets(weight);

        // WiFi Connection killed
        Serial.println("\nWiFi is disconnected");
        WiFi.disconnect();
    }
}

void accessToGoogleSheets(int weight) {
    HTTPClient http;
    String URL = "https://script.google.com/macros/s/";

    URL += KEY;
    URL += "/exec?";
    URL += "weight=";
    URL += weight;
    // URL += "&humidity=";
    // URL += humidity;
    // URL += "&illumination=";
    // URL += illumination;

    Serial.println("[HTTP] begin...");
    Serial.println(URL);
    // access to your Google Sheets
    Serial.println();
    // configure target server and url
    http.begin(URL);

    Serial.println("[HTTP] GET...");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.print("[HTTP] GET... code: ");
        Serial.println(httpCode);

        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.println(payload);
        }
    } else {
        Serial.print("[HTTP] GET... failed, error: ");
        Serial.println(http.errorToString(httpCode).c_str());
    }
}
