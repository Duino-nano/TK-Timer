/**
 *
 *	@file		setingWeb.cpp
 *
 *	@brief		設定用webアプリ
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/11/09
 *
 */
#include <Arduino.h>
#include "AgroSenseHub.hpp"
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include "setingWeb.hpp"
#include <stdio.h>
#include "eeprom.hpp"
#include "display.hpp"
#include "key.hpp"
/*==========================================================================================
	外部関数定義
==========================================================================================*/


/*==========================================================================================
	内部定数定義
==========================================================================================*/

/*==========================================================================================
	内部関数定義
==========================================================================================*/
/*==========================================================================================
	外部変数定義
==========================================================================================*/
/*==========================================================================================
	内部変数定義
==========================================================================================*/
/**
 *	@var		ssPins
 *	@brief		RFIDチャンネル
 */

/**
 *	@var		mfrc522
 *	@brief		各チャンネルインスタンス
 */
AsyncWebServer webServer(80);

ConfigData config;

/*==========================================================================================
	関数
==========================================================================================*/
void getData(char *data, AsyncWebServerRequest *request);
void handleRestart(AsyncWebServerRequest *request);
/**
 *	@brief		設定WEBアプリ処理（非同期処理）
 *	@param		なし
 *	@retval		なし
 */
void setingWeb() {
	WiFi.disconnect();
	delay(500);
	// Start SPIFFS
	if (!SPIFFS.begin(true)) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	// Set up Access Point
	WiFi.softAP(PRODUCT, "m2-labo00");
	Serial.println(WiFi.softAPIP());
	
	// Web server
	webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/index.html", "text/html");
	});
	

	webServer.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request){
		DynamicJsonDocument jsonDoc(1024);
		jsonDoc["ssid"] = config.wifi.ssid;
		jsonDoc["ps"] = config.wifi.ps;
		jsonDoc["gasId"] = config.gas.devId;
		jsonDoc["gasInterval"] = config.gas.interval;
		// JSONオブジェクトを文字列に変換
		String jsonResponse;
		serializeJson(jsonDoc, jsonResponse);
		Serial.println(jsonResponse);
		// レスポンスを送信
		request->send(200, "application/json", jsonResponse);
	});



	webServer.on("/ssidUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
		Serial.print("ssid:");
		getData(config.wifi.ssid, request);
	});

	webServer.on("/psUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
		Serial.print("ps:");
		getData(config.wifi.ps, request);
	});

	webServer.on("/gasIdUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
		Serial.print("gasId:");
		getData(config.gas.devId, request);
	});

	webServer.on("/gasIntervalUpdate", HTTP_POST, [](AsyncWebServerRequest *request){
		Serial.print("gasInterval:");
		Serial.println(request->arg("newData"));
		config.gas.interval= atoi(request->arg("newData").c_str());
		request->send(200);
		eepromSave();	
		// getData(config.gas.devId, request);
	});

	webServer.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request){
		handleRestart(request);
	});

	webServer.begin();
}

/**
 *	@brief		データ取得処理
 *	@param		data:書き込み先アドレス、request:rリクエストデータアドレス
 *	@retval		なし
 */
void getData(char *data, AsyncWebServerRequest *request){
	String newData = request->arg("newData");
	strcpy(data, newData.c_str());
	Serial.println(data);
	request->send(200);
	eepromSave();
}



/**
 *	@brief		再起動処理
 *	@param		request:rリクエストデータアドレス
 *	@retval		なし
 */
void handleRestart(AsyncWebServerRequest *request) {
	Serial.print("restart");
	config.flag = eCOMPLETE;
	eepromSave();
	request->send(200, "text/plain", "Restarting...");
	ESP.restart();   
}

/**
 *	@brief		設定モード移管処理
 *	@param		なし
 *	@retval		なし
 */
void setWifiMode(KeyInput &key){
    //設定フラグを確認
	if(config.flag == eSETING){
		setStrDisp("SET");
		startLodingDisp(1, 100);
		setingWeb();
		while(1){
			if(key.isKey()){
				config.flag = eCOMPLETE;
				endLodingDisp();
				break;
			}
		}
	}
}