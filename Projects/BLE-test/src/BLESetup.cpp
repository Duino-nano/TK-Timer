/**
 *
 *	@file		BLESetup.cpp
 *
 *	@brief		BLEセットアップ処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2024/05/15
 *
 */
#include "BLESetup.hpp"
#include "BLECallbacks.hpp"
#include "timer.hpp"
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
 *	@var	 serverCallbacks
 *	@brief	
 */
MyServerCallbacks* serverCallbacks = nullptr;
/*==========================================================================================
	関数
==========================================================================================*/


/**
 * @fn BLESetup::BLESetup(BLEServer* server)
 * @brief BLEセットアップ処理(インスタンス生成)
*/
BLESetup::BLESetup(BLEServer* server) : pServer(server) {
	bleServices = {
		// OTAサービス
		{
			OTA_UUID, 
			{
				{ VERSION_UUID	, BLE_READ						, "1.0.0"	, nullptr, new VersionCallbacks() }, // VERSION
				{ FILE_UUID			, BLE_WRITE						, "FILE"	, nullptr, new FileCallbacks() 		}, // FILE
				{ SELECT_UUID		, BLE_READ | BLE_WRITE, "SELECT", nullptr, new SelectCallbacks() 	}, // SELECT
			}
		},
		// コントロールサービス
		{
			CONTROL_UUID, 
			{
				{ XYS_UUID		, BLE_WRITE						, "XYSPEED"	, nullptr, new XYSpeedCallbacks() }, // XYSPEED
				{ WORK_UUID		, BLE_READ | BLE_WRITE, "WORK"		, nullptr, new WorkCallbacks() 		}, // WORK
				{ MODE_UUID		, BLE_WRITE						, "MODE"		, nullptr, new ModeCallbacks() 		}, // MODE
			}
		},
		// 設定サービス
		{
			SETTING_UUID, 
			{
				{ TYPE_UUID		, BLE_READ | BLE_WRITE, "TYPE"		, nullptr, new TypeCallbacks() 		}, // TYPE
				{ ROVER_UUID	, BLE_READ | BLE_WRITE, "ROVER"		, nullptr, new RoverCallbacks() 	}, // ROVER
				{ DRIVE_UUID	, BLE_READ | BLE_WRITE, "DRIVE"		, nullptr, new DriveCallbacks() 	}, // DRIVE
				{ RC_UUID			, BLE_READ | BLE_WRITE, "RC"			, nullptr, new RCCallbacks() 			}, // RC
				{ SAFETY_UUID	, BLE_READ | BLE_WRITE, "SAFETY"	, nullptr, new SafetyCallbacks() 	}, // SAFETY
			}
		},
		// POWERサービス
		{
			POWER_UUID, 
			{
				{ ONOFF_UUID	, BLE_WRITE						, "ONOFF"		, nullptr, new OnOffCallbacks() 	}, // ONOFF
				{ VOLTAGE_UUID, BLE_READ						, "VOLTAGE"	, nullptr, new DummyCallbacks() 	}, // VOLTAGE
				{ BATTERY_UUID, BLE_READ | BLE_WRITE, "BATTERY"	, nullptr, new BatteryCallbacks() }, // BATTERY
			}
		},
	};
}

/**
 * @fn void BLESetup::init()
 * @brief BLEセットアップ処理
*/
void BLESetup::init() {
	for (auto& serviceInfo : bleServices) {
		serviceInfo.service = pServer->createService(serviceInfo.uuid);
		for (auto& charInfo : serviceInfo.characteristics) {
			charInfo.characteristic = serviceInfo.service->createCharacteristic(
				charInfo.uuid, charInfo.properties);
			charInfo.characteristic->setValue(charInfo.initialValue);
			charInfo.characteristic->addDescriptor(new BLE2902());
			charInfo.characteristic->setCallbacks(charInfo.callbacks);
		}
		serviceInfo.service->start();
	}
	pServer->getAdvertising()->start();
	Serial.println("BLE Server is running");
}
// BLE UUIDの定義

/**
 * @fn void BLEStart()
 * @brief BLE開始処理
*/
void BLEStart() {
  Serial.begin(115200);  // シリアル通信を115200ボーレートで初期化
  BLEDevice::init("ESP32-BLE");

  BLEServer* pServer = BLEDevice::createServer();
  serverCallbacks = new MyServerCallbacks(pServer);
  pServer->setCallbacks(serverCallbacks); // 接続/切断コールバックの設定

  BLESetup bleSetup(pServer);
  bleSetup.init();

  BLEAddress macAddress = BLEDevice::getAddress();
  Serial.print("BLE MAC Address: ");
  Serial.println(macAddress.toString().c_str());  // MACアドレスをシリアルモニタに表示
}

/**
 * @fn void BLEMain()
 * @brief BLEメイン処理
*/
void BLEMain() {
	static IntervalTimer timer(500);
	static int sq = 0;

	switch(sq) {
		case 0:	// 接続状態の監視
			if (serverCallbacks && !serverCallbacks->deviceConnected) {
				if (serverCallbacks->oldDeviceConnected) {	// 切断時
					timer.setTime(500);	// 再接続前に少し待機
					sq++;
				}
			}
			break;

		case 1:
			if(timer.isWait()) {	// 待機後再接続
				serverCallbacks->pAdvertising->start();	// 再接続のために広告を再開
				Serial.println("Restarting advertising to reconnect");
				serverCallbacks->oldDeviceConnected = serverCallbacks->deviceConnected;
				timer.setTime(1000);	// 再接続を待つ
				sq++;
			}
			break;

		case 2:
			if(timer.isWait()) {	// 再接続待ち
				sq++;
			}
			break;

		case 3:
			// 接続時
			if (serverCallbacks && serverCallbacks->deviceConnected && !serverCallbacks->oldDeviceConnected) {
				Serial.println("Device connected");
				serverCallbacks->oldDeviceConnected = serverCallbacks->deviceConnected;
				sq = 0;
			}
			break;

		default:
			sq = 0;
			break;

	}
}