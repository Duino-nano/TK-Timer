#include <HardwareSerial.h>
#include "timer.hpp"
// SBUS信号を受信するシリアルポートの設定
// HardwareSerial sbusSerial(2); // 例: ESP32のSerial2を使用

// SBUSフレームデータを格納する配列
uint8_t sbusData[25] = {0};
// チャンネルデータを格納する配列（16チャンネル）
uint16_t channels[16] = {0};

IntervalTimer outTimer(100);

void setup() {
  // シリアル通信の初期設定
  Serial.begin(115200);
  // SBUS受信機の初期設定：100,000 bps、8E2フォーマット
  Serial2.begin(100000, SERIAL_8E2, 16, 17); // RXピンとTXピンを適切に設定
}


bool readSbusFrame() {
  while (Serial2.available() >= 24) {
    if (Serial2.read() == 0x0F) { // SBUSフレームの開始バイト
      Serial2.readBytes(sbusData, 24); // 残りのフレームを読み取る
      return true; // フレームが正常に読み取られた
    }
  }
  return false; // 完全なフレームが読み取られなかった
}

void decodeSbusData() {
  int bitCount = 0;
  int bitAddr = 0;
  uint32_t buff = 0;
  for(int i=0; i<16;){
    if(bitCount >= 11){
      channels[i++] = buff & 0x07FF;
      buff >>= 11;
      bitCount -= 11;
    }else{
      buff |= (sbusData[bitAddr] << bitCount);
      bitAddr++;
      bitCount += 8;
      if(bitAddr >= 24){
        Serial.println("over");
        return;
      }
    }
  }
}
void loop() {
  char data[124];
  uint8_t buff = 0;
  // SBUSフレームを読み取る
  if (readSbusFrame()) {
    // フレームからチャンネルデータをデコード
    if(outTimer.isWait()){
      decodeSbusData();
      // sprintf(data, " %4d: %4d: %4d: %4d: %4d: %4d: %4d: %4d: %4d: %4d: %4d: %4d: %4d: %4d: %4d: %4d",
      //         channels[0],channels[1],channels[2],channels[3],channels[4],channels[5],channels[6],channels[7],channels[8],channels[9],channels[10],channels[11],channels[12],channels[13],channels[14],channels[15]);
      //         // チャンネルの数だけループを回す
      data[0] = '\0';
      for (int i = 0; i < 16; i++) {
        // 各チャンネルの値をdataに追加
        sprintf(data + strlen(data), " %4d:", channels[i]);
      }

      // 最後のコロンを削除する場合（必要に応じて）
      data[strlen(data) - 1] = '\0';
      Serial.println(data);
    }
  }
}
