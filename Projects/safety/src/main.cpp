#include <Arduino.h>
#include "timer.hpp"
#include "sonar.hpp"
#include "can.hpp"
IntervalTimer it(500);
void setup(){
  Serial.begin(115200);  
  delay(1000);
  canBegin();
  initSonar();
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);

}
void loop(){
  getSonarData();
  if(it.isWait()){
    canIsSend();
    // シリアル出力
    for(int i = 0; i < 4; i++){
      // if((sonarData[i] < 50)&&(sonarData[i])){
      //   digitalWrite(14, HIGH);
      // }
      Serial.print("sonarData[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.print(sonarData[i]);
      Serial.println(" mm");
    }
  }
}
