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
// put function declarations here:
ModbusRTU mb;
int stcomp(const char* a, const char* b);
void serialStr(HardwareSerial& serial);
int getReceiveData(char *str);
void setup() {
  // // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  Serial.begin(9600);
  Serial2.begin(9600);
  mb.begin(&Serial2,RS485EN_PIN,true);
  mb.master();
  pinMode(TIMER_PIN,OUTPUT);
  pinMode(RS485EN_PIN,OUTPUT);
  digitalWrite(RS485EN_PIN,LOW);
  pinMode(GPS1_PIN,INPUT);
  pinMode(GPS2_PIN,INPUT);
  pinMode(EMG1_PIN,INPUT);
  pinMode(EMG2_PIN,INPUT);
  pinMode(RECV1_PIN,INPUT);
  pinMode(RECV2_PIN,INPUT);
  pinMode(RECV3_PIN,INPUT);
  pinMode(RECV4_PIN,INPUT);
  
}

void loop() {
  mb.readCoil()
  char data[256];
  // serialStr(Serial2,data);
  // stcomp(data,"A");
  serialStr(Serial2);
  if(getReceiveData(data)){
    Serial.println();
    Serial.println( data  );
    if(stcomp(data,"ABCD123\n")){
      digitalWrite(TIMER_PIN,HIGH);
    }else{
      digitalWrite(TIMER_PIN,LOW);
    }
  }else{
      digitalWrite(TIMER_PIN,LOW);
    }
  
  if(!digitalRead(   GPS1_PIN )){
    digitalWrite(RS485EN_PIN,HIGH);
    Serial.println( "GPS1_PIN"  );
    Serial2.print( "ABCD123\n"  );
    Serial2.flush();
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

int stcomp(const char* a, const char* b){
  int count = 0;
  while(*a == *b){
    if(*a=='\n'){
      return 1;
    }
    if(count > 256){
      break;
    }
    a++;
    b++;
    count++;
  }
  Serial.println(*a,HEX);
  Serial.println(*b,HEX);
  Serial.println();
  return 0;
}

// int serialStr(HardwareSerial& serial, char* str){
//   char data[256];
//   int count = 0;
//   while(serial.peek() != -1){
//     *data = serial.read();
//     if((*data == '\0')||(count > 255)){
//       break;
//     }
//     count++;
//     data++;
//   }
//   if(*data == '\0'){
//     while(count){
//       *str = *(data-count);
//       count--;
//       str++;
//     }
//     return 1;
//   }else{
//     return 0;
//   }
// }
char data[256];
int receiveFlag = 0;
void serialStr(HardwareSerial& serial) {
  static int count = 0;
  static char str[256];
  char ch;

  while (serial.available() > 0) {
    ch = serial.read();
    
    str[count] = ch;
    if (ch == '\n' || count >= 255) { // カウントの上限を設定
      Serial.println("END");
      Serial.println(ch,HEX);
      break;
    }
    count++;
    // Serial.println(ch);
  }
  if(ch == '\n'){
    for(int i=0; i<255; i++){
      data[i] = 0;
    }
    for(int i=0; i<=count; i++){
      data[i] = str[i];
    }
    for(int i=0; i<255; i++){
      str[i] = 0;
    }
    count = 0;
    receiveFlag = 1;
  }
}

int getReceiveData(char *str){
  int count = 0;
  if(receiveFlag){
    while(data[count] != '\0'){
      *str = data[count];
      str++;
      count++;
    }
    receiveFlag = 0;
  }
  return count;
}




