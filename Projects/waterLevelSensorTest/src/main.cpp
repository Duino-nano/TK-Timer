#include <Arduino.h>
#include "timer.hpp"
#include "avreage.hpp"

#define LED_PIN 13
#define SENSOR_OUT_PIN 14
#define SENSOR_IN_PIN 15

RingAverage ringAverage(100);
Average average1;
Average average2;
IntervalTimer timer(100);
unsigned long sensorRead();
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR_OUT_PIN, OUTPUT);
  pinMode(SENSOR_IN_PIN, INPUT);
  Serial.begin(9600);
  ringAverage.init();
  average1.bigin(10);
  average2.bigin(100);
}

void loop() {
   long data1 = 0;
   long data2 = 0;
  if(average1.main(sensorRead(),&data1)){
    if(average2.main(data1,&data2)){
      ringAverage.add(data2);
    }
  }
  if(timer.isWait()){
    Serial.println(ringAverage.get());
  }
}

unsigned long sensorRead() {
  unsigned long start = micros();
  for(int i = 0; i < 10; i++){
    digitalWrite(SENSOR_OUT_PIN, HIGH);
    while(!digitalRead(SENSOR_IN_PIN));
    digitalWrite(SENSOR_OUT_PIN, LOW);
    while(digitalRead(SENSOR_IN_PIN));
  }
  return (micros() - start);
}