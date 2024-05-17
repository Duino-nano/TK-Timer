/**
 *
 *	@file		BLECallbacks.hpp
 *
 *	@brief		BLEコールバック処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2024/05/15
 *
 */
#ifndef  __BLECALLBACKS_HPP__
#define __BLECALLBACKS_HPP__


#include <BLECharacteristic.h>
#include <BLEServer.h>

#include <Arduino.h>
/*==========================================================================================
	定数定義
==========================================================================================*/


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

/*==========================================================================================
	関数定義
==========================================================================================*/
/**
 * @class MyServerCallbacks
 * @brief BLEサーバーコールバック処理
*/
class MyServerCallbacks : public BLEServerCallbacks {
public:
    bool deviceConnected = false;
    bool oldDeviceConnected = false;
    BLEAdvertising* pAdvertising;

    MyServerCallbacks(BLEServer* server);
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
};

/**
 * @class DummyCallbacks
 * @brief ダミーコールバック
*/
class DummyCallbacks : public BLECharacteristicCallbacks {};

/**
 * @class VersionCallbacks
 * @brief バージョンコールバック
*/
class VersionCallbacks : public BLECharacteristicCallbacks {
	void onRead(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class FileCallbacks
 * @brief ファイルコールバック
*/
class FileCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class SelectCallbacks
 * @brief セレクトコールバック
*/
class SelectCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class XYSpeedCallbacks
 * @brief XYスピードコールバック
*/
class XYSpeedCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class ZSpeedCallbacks
 * @brief Zスピードコールバック
*/
class WorkCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class ModeCallbacks
 * @brief モードコールバック
*/
class ModeCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class TypeCallbacks
 * @brief タイプコールバック
*/
class TypeCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class RoverCallbacks
 * @brief ローバーコールバック
*/
class RoverCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class DriveCallbacks
 * @brief ドライブコールバック
*/
class DriveCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class RCCallbacks
 * @brief RCコールバック
*/
class RCCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class SafetyCallbacks
 * @brief セーフティコールバック
*/
class SafetyCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class OnOffCallbacks
 * @brief ON/OFFコールバック
*/
class OnOffCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};

/**
 * @class BatteryCallbacks
 * @brief バッテリーコールバック
*/
class BatteryCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) override;
};
#endif