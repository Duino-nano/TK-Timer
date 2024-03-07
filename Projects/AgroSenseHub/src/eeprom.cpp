/**
 *
 *	@file		eeprom.cpp
 *
 *	@brief		eeprom処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2024/01/15
 *
 */
#include <Arduino.h>
#include <EEPROM.h>
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

/*==========================================================================================
	関数
==========================================================================================*/
/**
 *	@brief		EEPROM初期設定
 *	@param		なし
 *	@retval		なし
 */
void eepromInit(void){
	EEPROM.begin(sizeof(config));

	EEPROM.get(0,config);
	Serial.print( "config.wifi.ssid: ");
	Serial.println(config.wifi.ssid);
	Serial.print( "config.wifi.ps: ");
	Serial.println(config.wifi.ps);
	Serial.print( "config.gas.devId: ");
	Serial.println(config.gas.devId);
	Serial.print( "config.gas.interval: ");
	Serial.println(config.gas.interval);
	Serial.print( "config.flag: ");
	Serial.println(config.flag);
	
	if(config.flag == eINITIAL){	//初期状態の場合は初期設定を書き込む
		strcpy(config.wifi.ssid, "SSID");
		strcpy(config.wifi.ps  , "PS");
		strcpy(config.gas.devId, "GASID");
		config.flag = eSETING;
		EEPROM.put(0, config);
		EEPROM.commit();
	}
}

/**
 *	@brief		EEPROM保存処理
 *	@param		なし
 *	@retval		なし
 */
void eepromSave(void){
	EEPROM.put(0, config);
	EEPROM.commit();
}