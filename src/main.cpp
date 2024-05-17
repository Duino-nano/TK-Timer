#include <Arduino.h>
#include "BLESetup.hpp"

// 
void setup() {
  Serial.begin(115200);  // シリアル通信を115200ボーレートで初期化
  BLEStart();
}

void loop() {
  // 必要な処理をここに追加
  BLEMain();
}