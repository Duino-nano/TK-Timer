/**
 *
 *	@file		BLESetup.hpp
 *
 *	@brief		BLEセットアップ処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2024/05/15
 *
 */
#ifndef  __BLESETUP_HPP__
#define __BLESETUP_HPP__

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <vector>
/*==========================================================================================
	定数定義
==========================================================================================*/
#define BLE_READ BLECharacteristic::PROPERTY_READ
#define BLE_WRITE BLECharacteristic::PROPERTY_WRITE
#define BLE_NOTIFY BLECharacteristic::PROPERTY_NOTIFY

// OTAのサービスUUIDとキャラクタリスティックUUIDの定義
#define OTA_UUID      "12345678-1234-1234-1234-1234567890ab"

#define VERSION_UUID  "12345678-1234-1234-1234-1234567890ac"
#define FILE_UUID     "12345678-1234-1234-1234-1234567890ad"
#define SELECT_UUID   "12345678-1234-1234-1234-1234567890ae"

// controlのサービスUUIDとキャラクタリスティックUUIDの定義
#define CONTROL_UUID  "12345678-1234-1234-1234-1234567890af"

#define XYS_UUID      "12345678-1234-1234-1234-1234567890b0"
#define WORK_UUID     "12345678-1234-1234-1234-1234567890b1"
#define MODE_UUID     "12345678-1234-1234-1234-1234567890b2"

// セッティングのサービスUUIDとキャラクタリスティックUUIDの定義
#define SETTING_UUID  "12345678-1234-1234-1234-1234567890b3"

#define TYPE_UUID     "12345678-1234-1234-1234-1234567890b4"
#define ROVER_UUID    "12345678-1234-1234-1234-1234567890b5"
#define DRIVE_UUID    "12345678-1234-1234-1234-1234567890b6"
#define RC_UUID       "12345678-1234-1234-1234-1234567890b7"
#define SAFETY_UUID   "12345678-1234-1234-1234-1234567890b8"

// POWERのサービスUUIDとキャラクタリスティックUUIDの定義
#define POWER_UUID    "12345678-1234-1234-1234-1234567890b9"

#define ONOFF_UUID    "12345678-1234-1234-1234-1234567890ba"
#define VOLTAGE_UUID  "12345678-1234-1234-1234-1234567890bb"
#define BATTERY_UUID  "12345678-1234-1234-1234-1234567890bc"

/*==========================================================================================
	変数定義
==========================================================================================*/
/**
 * @struct	MEMORY_DATA
 * @brief		メモリーデータ
 */


/**
 * @class 	IntervalTimer
 * @brief	Timer処理
 */

/**
 * @struct BLECharacteristicInfo
 * @brief キャラクタリスティック情報
 */
struct BLECharacteristicInfo {
    std::string uuid; // キャラクタリスティックのUUID
    uint32_t properties;  // キャラクタリスティックのプロパティ
    std::string initialValue; // キャラクタリスティックの初期値
    BLECharacteristic* characteristic;  // キャラクタリスティックのインスタンス
    BLECharacteristicCallbacks* callbacks;  // キャラクタリスティックのコールバック
};

/**
 * @struct BLEServiceInfo
 * @brief サービス情報
 */
struct BLEServiceInfo {
    std::string uuid; // サービスのUUID
    std::vector<BLECharacteristicInfo> characteristics; // キャラクタリスティックの情報を格納する配列
    BLEService* service;  // サービスのインスタンス
};

/**
 * @class BLESetup
 * @brief BLEセットアップ処理
 */
class BLESetup {
public:
    BLESetup(BLEServer* server);
    void init();

private:
    BLEServer* pServer;
    std::vector<BLEServiceInfo> bleServices;
};


/*==========================================================================================
	関数定義
==========================================================================================*/
extern void BLEStart();
extern void BLEMain();

#endif