#include <Arduino.h>
#include <ModbusRTU.h>
#define TIMER_PIN 25
#define GPS1_PIN  22
#define GPS2_PIN  23
#define EMG1_PIN  14
#define EMG2_PIN  13
#define RECV1_PIN 32
#define RECV2_PIN 33
#define RECV3_PIN 34
#define RECV4_PIN 35
#define RS485EN_PIN 18

#define REGN 1
#define SLAVE_ID 1
ModbusRTU mb;
Modbus::ResultCode result;


bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {

  Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
  result = event;  // リクエストの終了ステータスをresultにセットする
  return true;
}
void setup() {
  pinMode(TIMER_PIN,OUTPUT);
  pinMode(RS485EN_PIN,OUTPUT);
  pinMode(GPS1_PIN,INPUT);
  pinMode(GPS2_PIN,INPUT);
  pinMode(EMG1_PIN,INPUT);
  pinMode(EMG2_PIN,INPUT);
  pinMode(RECV1_PIN,INPUT);
  pinMode(RECV2_PIN,INPUT);
  pinMode(RECV3_PIN,INPUT);
  pinMode(RECV4_PIN,INPUT);
  Serial.begin(9600);
  Serial2.begin(9600);
  mb.begin(&Serial2,RS485EN_PIN,true);
  mb.slave(SLAVE_ID);  // 自分自身のスレーブIDを設定する
  mb.addIreg(REGN);  // Iregを使うことを指定する
}
bool data;
void loop() {
  static unsigned long lasttime = 0;
  if ((millis() - lasttime) > 1 * 100) {
      lasttime = millis();
      mb.Ireg(REGN, (uint16_t)digitalRead(RECV4_PIN));
  }
  mb.task();
}

