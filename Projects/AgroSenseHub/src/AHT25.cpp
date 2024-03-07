/**
 *
 *	@file		AHT25.cpp
 *
 *	@brief		温度湿度センサーAHT25処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2024/01/12
 *
 */
#include <Arduino.h>
#include <Wire.h>
#include <CRC8.h>
#include "timer.hpp"
#include "AHT25.hpp"
/*==========================================================================================
	外部関数定義
==========================================================================================*/
IntervalTimer wait(80);
/*==========================================================================================
	内部定数定義
==========================================================================================*/
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22
#define AHT25_ADDR  0x38

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


/*==========================================================================================
	関数
==========================================================================================*/

/**
 *	@brief		インタラクタ
 *	@param		pin:センサー有効ピン, t:更新時間
 *	@retval		なし
 */
AHT25::AHT25(int pin, unsigned long t, int n){
  enablePin = pin;  
  interval = t;
  tempAverage = new RingAverage(n, 1000);
  humiAverage = new RingAverage(n, 1000);
  pinMode(enablePin, OUTPUT);  
  crc.setPolynome(0x31);
  crc.setInitial(0xFF);
}

/**
 *	@brief		デスインタラクタ
 *	@param		なし
 *	@retval		なし
 */
AHT25::~AHT25(void){
  delete tempAverage;
  delete humiAverage;
}

/**
 *	@brief		センサーデータ読み取り処理
 *	@param		なし
 *	@retval		0：取得中　1：取得完了
 */
int AHT25::readData(void){
  static byte buf[7];
  unsigned long humidity_raw;
  unsigned long temperature_raw;
  int result = 0;
  static int sequence = 0;
  static int byteCount = 0;

  switch(sequence){
    case 0: //測定センサーへ通電開始
      digitalWrite(enablePin, HIGH);
      wait.setTime(interval);
      for(int i=0; i<7; i++){
        buf[i] = 0;
      }
      sequence++;
    break;

    case 1: //通電後安定まで待機
      if(wait.isWait()){
        Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
        sequence++;
      }
    break;

    case 2: //測定センサーの初期設定
      Wire.beginTransmission(AHT25_ADDR);
      Wire.write(0x71);
      Wire.endTransmission();
      wait.setTime(10);
      sequence++;
    break;

    case 3: //設定後待機
      if(wait.isWait()){
        sequence++;
      }
    break;
    
    case 4: //データ取得開始
      Wire.beginTransmission(AHT25_ADDR);
      Wire.write(0xAC); //トリガー測定
      Wire.write(0x33);
      Wire.write(0x00);
      Wire.endTransmission();
      wait.setTime(80);
      sequence++;
    break;

    case 5://取得コマンド送信後待機
      if(wait.isWait()){
        sequence++;
      }
    break;

    case 6: //データ取得開始
      Wire.requestFrom(AHT25_ADDR, 7); //7bytのデータを取得
      if (Wire.available() >= 7) {
        sequence++;
      }
    break;

    case 7: //データ取得
      if(byteCount < 7){
        buf[byteCount] = Wire.read();
        byteCount++;
      }else{
        byteCount = 0;
        sequence++;
      }
    break;
    
    case 8: //データ取得完了確認
      if((buf[0] & 0x80) != 0){
        sequence = 5;
      }else{
        sequence++;
      }
    break;

    case 9: //データ処理
      //CRCエラー確認
      crc.restart();
      crc.add(buf, 6);
      if (buf[6] == crc.getCRC()) {
        //湿度データ組立
        humidity_raw = ((uint32_t)buf[1] << 12);
        humidity_raw |= ((uint32_t)buf[2] << 4);
        humidity_raw |= (((uint32_t)buf[3] >> 4) & 0x0F);//20bit取得

        //湿度データ換算
        humidity_raw *= 100;      //百分率で表示
        humidity_raw /= 10486;    //最大値20bit/100(小数点2桁まで表示するため)
        humidity = (long)humidity_raw;
        humiAverage->add(humidity); //移動平均へ追加

        //温度データ組立
        temperature_raw = (((uint32_t)buf[3] & 0x0F) << 16);
        temperature_raw |= ((uint32_t)buf[4] << 8);
        temperature_raw |= ((uint32_t)buf[5]);//20bit取得

        //温度データ換算
        temperature_raw *= 200;     //二百分率で表示   
        temperature_raw /= 10486;   //最大値20bit/100(小数点2桁まで表示するため) 
        temperature = (long)temperature_raw - 5000;//-50℃から測定できるための下駄
        tempAverage->add(temperature);  //移動平均へ追加
      } else {
        // error
        humidity = ERROR_VALUE;
        temperature = ERROR_VALUE;
        tempAverage->init();
        tempAverage->add(ERROR_VALUE);
        humiAverage->init();
        humiAverage->add(ERROR_VALUE);
      }
      digitalWrite(enablePin, LOW);//センサーOFF
      result = 1;//取得完了フラグ
      sequence = 0;
    break;

    default:
    sequence = 0;
    break;
  }
  return result;
}

/**
 *	@brief		データプリント
 *	@param		なし
 *	@retval		なし
 */
void AHT25::printValues(void){
  if (temperature == ERROR_VALUE || humidity == ERROR_VALUE) return;

  Serial.print("temperature: ");
  Serial.println(tempAverage->get());
  Serial.print("humidity: ");
  Serial.println(humiAverage->get());
}

/**
 *	@brief		温度データ取得
 *	@param		なし
 *	@retval		温度データ(*100)
 */
long AHT25::getTemp(void){
  return tempAverage->get();
}

/**
 *	@brief		湿度データ取得
 *	@param		なし
 *	@retval		湿度データ(*100)
 */
long AHT25::getHumi(void){
  return humiAverage->get();
}

/**
 *	@brief		初期化
 *	@param		なし
 *	@retval		なし
 */
void AHT25::init(void){
  tempAverage->init();
  humiAverage->init();
}