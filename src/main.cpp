#include <Arduino.h>
#include <TimerOne.h>

// ピン定義
const int buzzerPin      = 12;
const int upButtonPin    = 14;
const int resetButtonPin = 15;
const int startButtonPin = 16;

const int upKey    = 0;
const int resetKey = 1;
const int startKey = 2;
// 明星チャルメラの曲の音階
int melody[] = {
    // ド   レ   ミー レ    ド　    ド    レ   ミ    レ  ド   レ　
    0, 523, 587, 659, 587, 523, 0, 523, 587, 659, 587, 523, 587, 0,
};
// 明星チャルメラの曲の音の長さ
int noteDurations[] = {
    1000, 150, 150, 450, 150, 150, 450, 150, 150, 150, 150, 150, 900, 1000,
};

const int segmentPins[] = {4, 5, 6, 7, 8, 9, 10, 11};  // セグメント表示用のピン
const int digitPins[]   = {3, 2};                      // 桁表示用のピン
const int segment[]     = {
    // セグメント表示用の数字
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111,  // 9
    0b00000000,  // 消灯
};
int segDot[] = {0, 0, 0};  // ドット表示用の配列

const int buttonPin[3] = {upButtonPin, resetButtonPin, startButtonPin};  // ボタンのピン

// 表示する数字
int dispNum = 0;

void displaydispNumWithTimer();
void buzzer(int stopFlg);
int getKey(int key);

// セットアップ
void setup() {
  // セグメント表示用のピンを出力に設定
  for (int i = 0; i < (sizeof(segmentPins) / sizeof(segmentPins[0])); i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  // 桁表示用のピンを出力に設定
  for (int i = 0; i < (sizeof(digitPins) / sizeof(digitPins[0])); i++) {
    pinMode(digitPins[i], OUTPUT);
  }

  // ボタンのピンを入力に設定
  for (int i = 0; i < (sizeof(buttonPin) / sizeof(buttonPin[0])); i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
  // ブザーのピンを出力に設定
  pinMode(buzzerPin, OUTPUT);

  // タイマタイマー割込みを設定
  Timer1.initialize(1000);  // 1msごとに割り込み
  Timer1.attachInterrupt(displaydispNumWithTimer);
  Timer1.start();

  tone(buzzerPin, melody[0], 100);
  tone(buzzerPin, melody[0], 50000);

  Serial.begin(9600);
}

// メイン処理
void loop() {
  static unsigned long oldTime = millis();
  static int countTime         = 0;
  static int startFlg          = 0;

  // アップボタンが押されたらタイマー停止時に限り1分ずつ足す
  if (getKey(upKey) == 1) {
    if (startFlg == 0) {
      if (countTime < 99 * 60) {
        countTime += 60;
      }
    }
  }

  // リセットボタンが押されたらタイマーをリセット
  if (getKey(resetKey) == 1) {
    countTime = 0;
    startFlg  = 0;
  }

  // スタートボタンが押されたらタイマーをスタート/ストップ
  if (getKey(startKey) == 1) {
    if (countTime > 0) {
      startFlg = 1 - startFlg;
    } else {
      startFlg = 0;
    }
  }

  // カウントダウン処理

  if (millis() - oldTime > 100) {
    oldTime = millis();
    if (startFlg == 1 && countTime > 0) {
      countTime--;
      segDot[0] = countTime % 2;
    } else {
      segDot[0] = 0;
    }
  }

  if (countTime >= 60) {  // 60秒以上の場合は分に変換
    dispNum = countTime / 60;
  } else {
    dispNum = countTime;
  }

  if (countTime == 0 && startFlg == 1) {
    buzzer(0);
  } else {
    buzzer(1);
  }
}

void buzzer(int stopFlg) {
  static int thisNote           = 0;
  static unsigned long oldTime  = millis();
  static unsigned long waitTime = 0;
  if (stopFlg == 1) {
    thisNote = 0;
    waitTime = 0;
    oldTime  = millis();
    noTone(buzzerPin);
    return;
  }
  if (millis() - oldTime > waitTime) {
    oldTime = millis();
    if (thisNote < (sizeof(melody) / sizeof(melody[0]))) {
      waitTime = noteDurations[thisNote];
      tone(buzzerPin, melody[thisNote], noteDurations[thisNote]);
      thisNote++;
    } else {
      thisNote = 0;
    }
  }
}

void displaydispNum(int dispNum, int seg) {
  bool pinState = false;
  for (int i = 0; i < (sizeof(segmentPins) / sizeof(segmentPins[0])); i++) {
    pinState = bool(segment[dispNum] & (1 << i));  // 1ビットずつシフトして表示するかどうかを判定
    digitalWrite(segmentPins[i], pinState);
    if (i == 7) {  // 8番目のピンにはドットを表示
      digitalWrite(segmentPins[i], segDot[seg]);
    }
  }
}

void displayDigit(int digit) {
  for (int i = 0; i < (sizeof(digitPins) / sizeof(digitPins[0])); i++) {
    if (i == digit) {
      digitalWrite(digitPins[i], HIGH);
    } else {
      digitalWrite(digitPins[i], LOW);
    }
  }
}

// タイマー割込みを使用したセグメント表示delayは使わない
void displaydispNumWithTimer() {
  static int digit = 0;
  static int onoff = 1;
  int num[]        = {0, 0};
  // 桁分け
  if (dispNum < 0) {  // 0未満の場合は0にする
    dispNum = 0;
    num[0]  = 0;
    num[1]  = 0;
  } else {                  // 0以上の場合は桁分け
    num[0] = dispNum % 10;  // 1の位
    num[1] = dispNum / 10;  // 10の位
  }

  if (digit == 0) {  // 1の位の処理
    if (onoff == 1) {
      displayDigit(0);
      displaydispNum(num[0], 0);
      onoff = 0;
    } else {
      displayDigit(3);        // 消灯
      displaydispNum(10, 2);  // 消灯
      onoff = 1;
      digit = 1;
    }
  } else if (digit == 1) {            // 10の位の処理
    if (onoff == 1 && num[1] != 0) {  // 10の位が0の場合は表示しない
      displayDigit(1);
      displaydispNum(num[1], 1);
      onoff = 0;
    } else {
      displayDigit(3);        // 消灯
      displaydispNum(10, 2);  // 消灯
      onoff = 1;
      digit = 0;
    }
  }
}

// ボタンの状態を取得(長押し対策)
int getKey(int key) {
  static int oldState[3] = {HIGH, HIGH, HIGH};           // 前のボタンの状態入れる配列
  int newState           = digitalRead(buttonPin[key]);  // ボタンの状態を取得
  int returnState        = 0;
  if (oldState[key] == HIGH && newState == LOW) {  // ボタンが押された瞬間(HIGH -> LOWになった瞬間)
    returnState = 1;
  } else {
    returnState = 0;
  }
  oldState[key] = newState;  // 状態を保存
  return returnState;
}