/**
 *
 *	@file		gas.cpp
 *
 *	@brief		gas処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/12/25
 *
 */
#include <Arduino.h>
#include "gas.hpp"
#include <HTTPClient.h>
#include "setingWeb.hpp"

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
int gasStatus = 0;
/*==========================================================================================
	関数
==========================================================================================*/
/**
 *	@brief		wifi接続処理
 *	@param		なし
 *	@retval		経過時間(ms)
 */
int wifiConect(){
	unsigned long backTime = millis();
	
	while (WiFi.status() != WL_CONNECTED) {
		if(!(millis() - backTime)){     //初期時の処理
			WiFi.begin(config.wifi.ssid, config.wifi.ps);
			Serial.print("WiFi connecting"); 
		}
		if((millis() - backTime) > 15000){      //15秒を超えるとタイムアップ
			gasStatus = 1;
			break;
		}
		if(!((millis() - backTime)%5000)){     //5秒毎にwifi再接続
			Serial.println("WiFirestart");
			Serial.print("ssid: ");
			Serial.println(config.wifi.ssid);
			Serial.print("ps: ");
			Serial.println(config.wifi.ps);
			Serial.println();
			WiFi.disconnect();
			delay(100);
			WiFi.begin(config.wifi.ssid, config.wifi.ps);
		}
		delay(1);
	}
	Serial.println("\nconnected");
	return ((millis() - backTime) > 1500);
}

/**
 *	@brief		GASアップロード処理
 *	@param		weight：重量データ
 *	@retval		なし
 */
void accessToGoogleSheets(long *data, int size) {
	HTTPClient http;
	String URL = "https://script.google.com/macros/s/";
	char strBuff[10];
	URL += config.gas.devId;
	URL += "/exec?";
	for(int i=0; i<size; i++){
		sprintf(strBuff, "data%d=",i);
		URL +=strBuff;
		URL +=data[i];
		URL +="&";
	}
	// URL += "weight=";
	// URL += weight;
	// URL += "&userId=";
	// URL += userId;
	
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
		gasStatus = 0;
				// file found at server
		if(httpCode == HTTP_CODE_OK) {
			String payload = http.getString();
			Serial.println(payload);
		}
	} else {
		gasStatus = 2;
		Serial.print("[HTTP] GET... failed, error: ");
		Serial.println(http.errorToString(httpCode).c_str());
	}
}
