#include <Arduino.h>
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
// put function declarations here:

void setup() {
  // // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  Serial.begin(9600);
  Serial2.begin(9600);
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
  digitalWrite(TIMER_PIN,LOW);
  delay(1000);
  digitalWrite(TIMER_PIN,HIGH);
}

void loop() {
  if(!digitalRead(   GPS1_PIN )){
    digitalWrite(RS485EN_PIN,HIGH);
    Serial.println( "GPS1_PIN"  );
    Serial.print( "A"  );
    delay(100);
    digitalWrite(RS485EN_PIN,LOW);
  }
  if(!digitalRead(   GPS2_PIN )){
    Serial.println( "GPS2_PIN"  );
  }
  if(!digitalRead(   EMG1_PIN )){
    Serial.println( "EMG1_PIN"  );
  }
  if(!digitalRead(   EMG2_PIN )){
    Serial.println( "EMG2_PIN"  );
  }
  if(!digitalRead(   RECV1_PIN )){
    Serial.println( "RECV1_PIN"  );
  }
  if(!digitalRead(   RECV2_PIN )){
    Serial.println( "RECV2_PIN"  );
  }
  if(!digitalRead(   RECV3_PIN )){
    Serial.println( "RECV3_PIN"  );
  }
  if(!digitalRead(   RECV4_PIN )){
    Serial.println( "RECV4_PIN"  );
  }
  // put your main code here, to run repeatedly:
}

