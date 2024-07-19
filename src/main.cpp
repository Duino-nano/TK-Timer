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
const int melody[] = {
    // ド   レ   ミー レ    ド　    ド    レ   ミ    レ  ド   レ　
    0, 523, 587, 659, 587, 523, 0, 523, 587, 659, 587, 523, 587, 0,
};
// 明星チャルメラの曲の音の長さ
const int noteDurations[] = {
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

// 表示する数字　ここに表示したい数字を入れる
int dispNum = 0;

// 関数の宣言
void displaydispNumWithTimer();
void buzzer(int stopFlg);
int getKey(int key);

// セットアップ　最初に一度だけ実行される
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

// メイン処理　繰り返し実行される
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
    if (startFlg == 1 && countTime > 0) {  // 　カウント中の処理
      countTime--;                         // 1秒ずつ減らす
      segDot[0] = countTime % 2;           // 1秒毎にドットを点滅
    } else {
      segDot[0] = 0;  // カウント中でない場合はドットを消灯
    }
  }

  // 表示数の処理
  if (countTime >= 60) {  // 60秒以上の場合は分に変換
    dispNum = countTime / 60;
  } else {
    dispNum = countTime;
  }

  // カウントダウンが0になったらブザーを鳴らす
  if (countTime == 0 && startFlg == 1) {
    buzzer(0);
  } else {
    buzzer(1);
  }
}

// ここから下は関数の定義

// ブザーを鳴らす関数　stopFlgが1の場合はブザーを止める
void buzzer(int stopFlg) {
  static int thisNote           = 0;         // 今の音階
  static unsigned long oldTime  = millis();  // 時間計測用のバッファ
  static unsigned long waitTime = 0;         // 次の音階までの時間
  if (stopFlg == 1) {                        // stopFlgが1の場合はブザーを止めて初期化
    thisNote = 0;
    waitTime = 0;
    oldTime  = millis();
    noTone(buzzerPin);  // ブザーを止める
    return;             // 関数を抜ける
  }
  if (millis() - oldTime > waitTime) {                             // 次の音階までの時間が経過したら次の音階を鳴らす
    oldTime = millis();                                            // 時間をリセット
    if (thisNote < (sizeof(melody) / sizeof(melody[0]))) {         // 最後の音階まで行ったら初期化
      waitTime = noteDurations[thisNote];                          // 次の音階までの時間を設定
      tone(buzzerPin, melody[thisNote], noteDurations[thisNote]);  // 音階を鳴らす
      thisNote++;                                                  // 次の音階に進む
    } else {
      thisNote = 0;
    }
  }
}

// セグメント表示関数　dispNumに表示したい数字を入れる　segには表示する桁を入れる
void displaydispNum(int dispNum, int seg) {
  bool pinState = false;
  for (int i = 0; i < (sizeof(segmentPins) / sizeof(segmentPins[0])); i++) {  // セグメントの各ピンに対して処理
    pinState = bool(segment[dispNum] & (1 << i));                             // 1ビットずつシフトして表示するかどうかを判定
    digitalWrite(segmentPins[i], pinState);                                   // ピンの状態を設定
    if (i == 7) {                                                             // 8番目のピンにはドットを表示
      digitalWrite(segmentPins[i], segDot[seg]);                              // ドット表示
    }
  }
}

// 桁表示関数　桁を指定して表示する　0が1の位　1が10の位
void displayDigit(int digit) {
  for (int i = 0; i < (sizeof(digitPins) / sizeof(digitPins[0])); i++) {  // 桁の各ピンに対して処理
    if (i == digit) {                                                     // 表示する桁のピンをHIGHにする
      digitalWrite(digitPins[i], HIGH);
    } else {
      digitalWrite(digitPins[i], LOW);
    }
  }
}

// タイマー割込みから呼ばれるセグメント表示関数　1msごとに呼ばれる
void displaydispNumWithTimer() {
  static int digit = 0;
  static int onoff = 1;       // セグメントを消灯するかどうかのフラグ
  int num[]        = {0, 0};  // 表示する数字を桁分けしたものを入れる配列

  // 桁分け 1の位と10の位に分ける
  if (dispNum < 0) {  // 0未満の場合は0にする
    dispNum = 0;
    num[0]  = 0;
    num[1]  = 0;
  } else {                  // 0以上の場合は桁分け
    num[0] = dispNum % 10;  // 1の位
    num[1] = dispNum / 10;  // 10の位
  }

  // 1msごとに呼ばれるので1msごとに表示処理をする。一桁目表示→消灯→二桁目表示→消灯の順で表示(消灯は残像対策)
  if (digit == 0) {               // 1の位の処理
    if (onoff == 1) {             // 1の場合セグメントを表示
      displayDigit(0);            // 1の位の桁を表示
      displaydispNum(num[0], 0);  // 1の位の数字を表示
      onoff = 0;                  // 次の割り込みで消灯するようにする
    } else {
      displayDigit(3);        // 消灯
      displaydispNum(10, 2);  // 消灯
      onoff = 1;              // 次の割り込みで表示するようにする
      digit = 1;              // 10の位の処理に移る
    }
  } else if (digit == 1) {            // 10の位の処理
    if (onoff == 1 && num[1] != 0) {  // 10の位が0の場合は表示しない
      displayDigit(1);                // 10の位の桁を表示
      displaydispNum(num[1], 1);      // 10の位の数字を表示
      onoff = 0;                      // 次の割り込みで消灯するようにする
    } else {
      displayDigit(3);        // 消灯
      displaydispNum(10, 2);  // 消灯
      onoff = 1;              // 次の割り込みで表示するようにする
      digit = 0;              // 1の位の処理に移る
    }
  }
}

// ボタンの状態を取得(長押し対策)
int getKey(int key) {
  static int oldState[3] = {HIGH, HIGH, HIGH};           // 前のボタンの状態入れる配列
  int newState           = digitalRead(buttonPin[key]);  // ボタンの状態を取得
  int returnState        = 0;                            // 戻り値
  if (oldState[key] == HIGH && newState == LOW) {        // ボタンが押された瞬間(HIGH -> LOWになった瞬間)
    returnState = 1;                                     // 戻り値を1にする
  } else {
    returnState = 0;  // ボタンが押された瞬間以外は戻り値を0にする
  }
  oldState[key] = newState;  // 状態を保存
  return returnState;        // 戻り値を返す
}