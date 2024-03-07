#include <Arduino.h>
#include <M5Stack.h>


#include <measure.h>
// put function declarations here:
void setup(){
  M5.begin();

  M5.Lcd.setCursor(20,60);
  M5.Lcd.setTextSize(4);
  M5.Lcd.print("Hello world");
  initMeasure();
  delay(1000);
  M5.Lcd.clear(BLACK);
}

void loop(){
  char data[50];
  sprintf(data, "%d", measure());
  M5.Lcd.setCursor(20,60);
  M5.Lcd.print(data);
  delay(1000);
}