# 1 "C:\\Users\\kyama\\AppData\\Local\\Temp\\tmpdpd9yle9"
#include <Arduino.h>
# 1 "C:/Users/kyama/OneDrive/ドキュメント/PlatformIO/Projects/M5stack/src/main.ino"
#include <Arduino.h>
#include <M5Stack.h>
#include measure.h
void setup();
void loop();
#line 5 "C:/Users/kyama/OneDrive/ドキュメント/PlatformIO/Projects/M5stack/src/main.ino"
void setup(){
  M5.begin();

  M5.Lcd.setCursor(20,60);
  M5.Lcd.setTextSize(4);
  M5.Lcd.print("Hello world");
  delay(1000);
}

void loop(){
  char data[10];
  sprintf(data, meauser());
  M5.Lcd.print(data);
  delay(1000);
}