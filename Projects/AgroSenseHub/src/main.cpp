// #include <Arduino.h>
// #include "AgroSenseHub.hpp"
// #include "AHT25.hpp"


// AHT25 sensor[] = {AHT25(IN_SENSOR_PIN, 1500),
//                   AHT25(OUT_SENSOR_PIN, 1500)};
// void setup(void) {
//   Serial.begin(9600);

// }

// void loop() {
//   static int i = 0;
//   if(sensor[i].getData()){
//     Serial.println(i);
//     sensor[i].printValues();
//     Serial.println();
//     // i = 1 - i;
//   }
// }

/**
 *
 *	@file		  main.cpp
 *
 *	@brief		AgroSenseHubメイン処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2024/01/17
 *
 */
#include <Arduino.h>
#include "AgroSenseHub.hpp"
#include "display.hpp"
#include <HTTPClient.h>
#include "avreage.hpp"
#include "setingWeb.hpp"
#include "timer.hpp"
#include "gas.hpp"
#include "eeprom.hpp"
#include "key.hpp"
#include "AHT25.hpp"
/*==========================================================================================
	内部定数定義
==========================================================================================*/


/*==========================================================================================
	内部変数定義
==========================================================================================*/


const char* APP_SERVER = "script.google.com";

IntervalTimer dispChangeTimer(3000);
IntervalTimer gasInterval(60000);
WiFiServer server(80);
KeyInput setPush(SET_BUTTON, 100);
KeyInput chPush(ch_BUTTON);

AHT25 sensor[] = {AHT25(IN_SENSOR_PIN, 1500),
                  AHT25(OUT_SENSOR_PIN, 1500)};
/**
 *	@brief		初期化
 *	@param		なし
 *	@retval		なし
 */
void setup() {
	//各デバイス初期化設定
	Serial.begin(9600);
	delay(1000);
	eepromInit();
	dispInit();
	setDotDisp(0,0,0,0);
	setFloatDisp(2);
	setLed(0b0000);

	//初期化表示
	setStrDisp("inT");
	startLodingDisp(1, 100);

	//wifi接続
	wifiConect();
	endLodingDisp();
  gasInterval.setTime(config.gas.interval*1000);
}

/**
 *	@brief		メイン処理
 *	@param		なし
 *	@retval		なし
 */
void loop() {
	char userId[25];
	int weight = 0;
	char led = 0b0000;
  static long sensorData[CH_MAX];
	static int sensorCh = 0;
  static int dispCh = 0;

  //温度湿度データ取得
  if(sensor[sensorCh].readData()){
    Serial.println(sensorCh);
    sensor[sensorCh].printValues();
    Serial.println();
    sensorData[sensorCh*2] = sensor[sensorCh].getTemp();
    sensorData[(sensorCh*2)+1] = sensor[sensorCh].getHumi();
    sensorCh = 1 - sensorCh;
  }

  if(chPush.isKey()||dispChangeTimer.isWait()){
    dispChangeTimer.init();
    dispCh++;
    if(dispCh >= CH_MAX){
      dispCh = 0;
    }

    setLed(0b0001 << dispCh);
    if(sensorData[dispCh] == ERROR_VALUE){
      setStrDisp("----");
    }else{
      if(sensorData[dispCh] > 0){
        setNumDisp((sensorData[dispCh]+5)/10);  //四捨五入して小数一桁で表示
      }else{
        setNumDisp((sensorData[dispCh]-5)/10);  //四捨五入して小数一桁で表示
      }
    }
  }

  if(gasInterval.isWait()){
    setStrDisp("UPro");	//アップロード画面を表示
    startLodingDisp(1, 100);
    wifiConect();
    accessToGoogleSheets(sensorData, sizeof(sensorData)/sizeof(sensorData[0]));
    endLodingDisp();
  }

  if(gasStatus == 1){
		setStrDisp("Er01");
	}else if(gasStatus == 2){
		setStrDisp("Er02");
	}
  
  if(setPush.isKey()){
		config.flag = eSETING;
	}
  setWifiMode(setPush);
}




















// 	if(fgScale.updateflg == 2){	//重量データ取得が完了したら
// 		weight = int(fgScale.data*10);	//重量値取得
// 		setNumDisp(weight);		//重量表示
// 		if(!strcmp(fgScale.status, "ST")){  //データが安定している場合
// 			led = getLed();
// 			led |= 0b1000;	
// 			setLed(led);	//安定LEDを点灯
// 			if (userRFID.readUid(userId)) {	//NFCをタッチした場合は
// 				setStrDisp("UPro");	//アップロード画面を表示
// 				startLodingDisp(1, 100);
// 				Serial.println(wifiConect());
// 				accessToGoogleSheets(weight, userId);
// 				endLodingDisp();
// 			}
// 		}else {
// 			led = getLed();
// 			led &= 0b0111;
// 			setLed(led);
// 		}
// 	}else if(gasStatus == 1){
// 		setStrDisp("Er01");
// 	}else if(gasStatus == 2){
// 		setStrDisp("Er02");
// 	}else if(!strcmp(fgScale.status, "Error")){
// 		setStrDisp("Er11");
// 	}

// 	if(setPush.isKey()){
// 		config.flag = eSETING;
// 	}
// 	if(zeroPush.isKey()){
// 		setStrDisp("CAL");
// 		startLodingDisp(1, 100);
// 		Serial2.print("Z\r\n");
// 		endLodingDisp();
// 	}
// 	setWifiMode(setPush);
// }

