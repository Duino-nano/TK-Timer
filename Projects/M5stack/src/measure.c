/*************************************************************
 * measure.c
 * 重量測定処理
 * 
**************************************************************/
#include <Arduino.h>
#define DATA_PIN    22
#define CLOCK_PIN   21

#define CLOCK_OUT(x) digitalWrite(CLOCK_PIN, x)
#define DATA_READ digitalRead(DATA_PIN)

void initMeasure();
void clockOut();
unsigned long measure();
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
			data = data << 1;
			data += DATA_READ;
    }
    clockOut();
    return data^0x800000;
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