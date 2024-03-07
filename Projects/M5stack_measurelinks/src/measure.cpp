/*************************************************************
 * measure.c
 * 重量測定処理
 * 
**************************************************************/
#include <Arduino.h>
#define DATA_PIN    36
#define CLOCK_PIN   26

#define CLOCK_OUT(x) digitalWrite(CLOCK_PIN, x)
#define DATA_READ digitalRead(DATA_PIN)

void initMeasure();
void clockOut();
unsigned long measure();
unsigned long average(int n);

unsigned long zeroAD = 0;

/** ==========================================================================================
 *	@brief	    測定処理初期化
 *	@param		なし
 *	@retval		なし
==========================================================================================*/
void initMeasure(){
    pinMode(DATA_PIN, INPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    CLOCK_OUT(1);
    delayMicroseconds(100);
    CLOCK_OUT(0);
    delayMicroseconds(100);
    measure();
    zeroAD = average(10);
}

/** ==========================================================================================
 *	@brief	    測定処理初期化
 *	@param		なし
 *	@retval		なし
==========================================================================================*/
unsigned long measure(){
    unsigned long data = 0;
    while(DATA_READ);

    for(int i=0; i<24; i++){
        clockOut();
        if(i < 24){
            data = data << 1;
            data += DATA_READ;
        }
    }
    clockOut();
    //マイナスの場合の処理
    Serial.println(data,BIN); 
    Serial.println(data&0x800000,BIN); 
    if(data&0x800000){ //先頭bitの値を確認
        Serial.println("minus"); 
        Serial.println(data,BIN); 
        data = ~data;   //bit反転
        data = data & 0xFFFFFF;
        Serial.println(data,BIN); 
        data + 1;   
        data = 0x7FFFFF - data; 
               
    }else{
        data = 0x7FFFFF + data; 
    }
    return data;
}

/** ==========================================================================================
 *	@brief	  クロック出力
 *	@param		なし
 *	@retval		なし
==========================================================================================*/
void clockOut(){
	CLOCK_OUT(1);
	delayMicroseconds(10);
	CLOCK_OUT(0);  
	delayMicroseconds(10);
}

/** ==========================================================================================
 *	@brief	    測定値平滑化（最大最小を除く平均）
 *	@param		n:平滑データ数　3〜100
 *	@retval		平均化AD n値範囲外の場合は０
==========================================================================================*/
unsigned long average(int n){
    int max = 0;
    int min = 0;
    int i = 0;
    unsigned long data = 0;
    if((n >= 3)&&(n <= 10)){
        /**ｎ回データを取得**/
        for(i=0;i<=n;i++){
            data += measure();
            /**初回処理**/
            if(!i){
                max = data;
                min = data;
            }
            /**最大最小を取得**/
            if(data > max)  max = data;
            if(data < min)  min = data;
        }
        /**最大最小を除き平均化**/
        data -= (max+min);
        data /= (n-2);
    }
    Serial.print("data:");
    Serial.println(data);
    Serial.print("zero:");
    Serial.println(zeroAD);
    Serial.println();
    return data - zeroAD;
    // return 0;
}
