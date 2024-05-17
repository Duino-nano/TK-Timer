#include <Update.h>
#include "BluetoothSerial.h"
#include "ArduinoOTA.h"
#include "base64.hpp"
#include "timer.hpp"
#include <CRC32.h>
#define XON 0x11
#define XOFF 0x13
BluetoothSerial SerialBT;
IntervalTimer btTimeOut(1000);

void setup() {
  Serial.begin(115200); // シリアル通信の開始
  SerialBT.begin("ESP32_BT_Receiver"); // Bluetoothデバイス名を設定

  Serial.println("Bluetooth Device is Ready to Pair");
}

void loop() {
  static const int bufSize = 512; // 必要に応じてサイズを調整(最大512バイト)
  char buf;
  static bool isXON = true;
  char btData[bufSize];
  uint8_t base64Data[bufSize];
  int dataSize = 0;
  int i = 0;
  static bool upDataFg = false;
  static CRC32 crc;
  static uint32_t crcValue = 0;
  
  // データ受信がある場合
  while (SerialBT.available()) {
    isXON = false;
    while(1){
      if(SerialBT.available()){
        btTimeOut.init(); // タイムアウトカウントをリセット
        buf = (char)SerialBT.read();
        if(buf == '\n'){  // 改行コードが来たら終了
          btData[i] = '\0';
          break;
        }else{
          btData[i++] = buf;
        }
        if(i >= bufSize){ // バッファサイズを超えたら終了
          break;
        }
      }
    }
    dataSize = base64Decode(btData, i, base64Data, 512);  // base64デコード
    i = 0;
    if(!upDataFg){  // ファームウェアアップデートの初期処理
      upDataFg = true;
      Update.begin();
      crc.reset();

      // 通信の冒頭にCRC値が送られてくるので、それを取得
      crcValue = 0; 
      for(int i = 0; i < 4; i++){
        crcValue <<= 8;
        crcValue |= base64Data[i];
      }
      Serial.print("crcValue");
      Serial.println(crcValue);
    }else{  // ファームウェアアップデートのデータ受信処理
      crc.update(base64Data, dataSize);
      Update.write(base64Data, dataSize);
    }
  }
  if(!isXON){ // 受信データの処理が完了したらXONを送信
    SerialBT.write(XON);
    isXON = true;
  }
  if(btTimeOut.isWait() && upDataFg){ // タイムアウトが発生したらファームウェアアップデートを実行
    upDataFg = false;
    Serial.println(crcValue);
    Serial.println(crc.finalize());
    if(crcValue == crc.finalize()){ // CRC値が一致しているか確認
      if (Update.end(true)) {
        Serial.println("更新しました。再起動します。");
        ESP.restart();
      } else {
        Serial.println("更新に失敗しました。");
      }
    }else{
      Serial.println("CRC値が一致しません。");
    }
  }
}