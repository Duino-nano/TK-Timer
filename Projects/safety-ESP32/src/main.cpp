#include <Arduino.h>
#include "timer.hpp"
typedef struct {
  uint8_t   address;
  uint8_t   type;
  uint16_t  cmd;
  uint8_t   size;
  uint32_t  data;
  uint16_t  crc;
}RS485DATA;
RS485DATA rs485data;
// unsigned char Com[8] = {0x01,0x03,0x01,0x01,0x00,0x01,0xd4,0x36};
int readDistance(uint8_t addr);
int readN(uint8_t *buffer, size_t len);
unsigned int CRC16_2(unsigned char *buf, int len);
void setup(){
  Serial.begin(9600);  
  Serial2.begin(115200); 

  delay(1000);
  unsigned char Com[8] = {0x05,0x06,0x02,0x00,0x00,0x01,0x48,0x71};
  Serial2.write(Com, 8);
  readN(Com, sizeof(Com));
   

}
void loop(){
  int Distance =readDistance(0x01);
  Serial.print("Distance1 = ");
  Serial.print(Distance);
  Serial.println(" mm");
  Distance =readDistance(0x05);
  Serial.print("Distance2 = ");
  Serial.print(Distance);
  Serial.println(" mm");
  delay(500);
}

int readDistance(uint8_t addr){
  unsigned char Com[8] = {addr,0x03,0x01,0x01,0x00,0x01,0xD4,0x36}; //距離取得コマンド
  unsigned char Data[16] = {0};
  int Distance = 0;
  int dataCount = 0;
  uint16_t crc;
  // crc = CRC16_2(Com,6);
  // Com[6] = (crc>>8) & 0xFF;
  // Com[7] = crc & 0xFF;
  // for(int i = 0; i < 8; i++){
  //   Serial.print(Com[i],HEX);
  //   Serial.print(" ");
  // }
  
  Serial2.write(Com, 8);
  dataCount = readN(Data, sizeof(Data));
  if(dataCount){  //データ取得したらデータ換算開始
    //CRCチェック
    if (CRC16_2(Data, dataCount-1) == ((Data[dataCount-1] << 8) | Data[dataCount])) {
      //コマンド内容確認
      for(int i = 0; i < 2; i++){
        if (Data[i] != Com[i]) { 
          return Distance;
        }
      }
      //データ取得
      for(int i = 0; i < Data[2]; i++){
        Distance <<= 8;
        Distance |= Data[3+i];
      }
    }
  }
  return Distance;
}

int readN(uint8_t *buffer, size_t len){
  int offset = 0;
  const uint32_t endCount = 305; //データ取得後3.5文字分待機して終了
  // uint8_t  buffer[16];
  uint32_t curr = micros();
  IntervalTimer timeOut(500);
  while (offset < len) {
    if (Serial2.available()) {
      curr = micros();
      if(offset < len){
        buffer[offset] = Serial2.read();
        Serial.print(buffer[offset],HEX);
        Serial.print(" ");
        offset++;
      }
    }
    //3.5文字分の空白で取得終了
    if ((micros() - curr) > endCount) {
      offset--;//文字数調整
      break;
    }
    //取得に時間がかかりすぎた場合は終了
    if(timeOut.isWait()){
      offset = 0;
      break;
    }
  }
  Serial.println();
  return offset;
}

unsigned int CRC16_2(unsigned char *buf, int len){
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++){
    crc ^= (unsigned int)buf[pos];
    for (int i = 8; i != 0; i--){
      if ((crc & 0x0001) != 0){
        crc >>= 1;
        crc ^= 0xA001;
      }else{
        crc >>= 1;
      }
    }
  }

  crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
  return crc;
}