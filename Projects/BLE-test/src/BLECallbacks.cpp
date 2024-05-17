/**
 *
 *	@file		BLECallbacks.cpp
 *
 *	@brief		BLEコールバック処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2024/05/15
 *
 */
#include "BLECallbacks.hpp"
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
 *	@var		
 *	@brief	
 */

/*==========================================================================================
	関数
==========================================================================================*/

/**
 *	@brief		workメイン処理
 *	@param		なし
 *	@retval		なし
 */

// BLE UUIDの定義

/**
 * @fn MyServerCallbacks::MyServerCallbacks(BLEServer* server)
 * @brief BLEサーバーコールバック処理（インスタンス生成）
*/
MyServerCallbacks::MyServerCallbacks(BLEServer* server) {
  pAdvertising = server->getAdvertising();
}

/**
 * @fn void onConnect(BLEServer* pServer)
 * @brief BLE接続時の処理
*/
void MyServerCallbacks::onConnect(BLEServer* pServer) {
  Serial.println("Client Connected");
  deviceConnected = true;
}

/**
 * @fn void onDisconnect(BLEServer* pServer)
 * @brief BLE切断時の処理
*/
void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
  Serial.println("Client Disconnected");
  deviceConnected = false;
  pAdvertising->start(); // 再接続のために広告を再開
}


/**
 * @fn void VersionCallbacks::onRead(BLECharacteristic *pCharacteristic)
 * @brief バージョンコールバック処理
*/
void VersionCallbacks::onRead(BLECharacteristic *pCharacteristic) {
  static int versionCount = 0;
  versionCount++;
  std::string newVersion = "1.0." + std::to_string(versionCount);
  pCharacteristic->setValue(newVersion);
  Serial.println("Version updated to: " + String(newVersion.c_str()));
}

/**
 * @fn void FileCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief ファイルコールバック処理
*/
void FileCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("FileCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void SelectCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief セレクトコールバック処理
*/
void SelectCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("SelectCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void XYSpeedCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief XYスピードコールバック処理
*/
void XYSpeedCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("XYSpeedCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void WorkCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief Workコールバック処理
*/
void WorkCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("WorkCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void ModeCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief モードコールバック処理
*/
void ModeCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("ModeCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void TypeCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief タイプコールバック処理
*/
void TypeCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("TypeCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void RoverCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief ローバーコールバック処理
*/
void RoverCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("RoverCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void DriveCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief ドライブコールバック処理
*/
void DriveCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("DriveCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void RCCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief RCコールバック処理
*/
void RCCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("RCCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void SafetyCallbacks::onWrite(BLECharacteristic *pCharacteristic)
 * @brief セーフティコールバック処理
*/
void SafetyCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("SafetyCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void DummyCallbacks::onRead(BLECharacteristic *pCharacteristic)
 * @brief ダミーコールバック処理
*/
void OnOffCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("OnOffCallbacks: Data received: " + String(value.c_str()));
}

/**
 * @fn void BatteryCallbacks::onRead(BLECharacteristic *pCharacteristic)
 * @brief バッテリーコールバック処理
*/
void BatteryCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  std::string value = pCharacteristic->getValue();
  Serial.println("BatteryCallbacks: Data received: " + String(value.c_str()));
}