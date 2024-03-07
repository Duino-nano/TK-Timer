/**
 *
 *	@file		  display.cpp
 *
 *	@brief		セグメント表示
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/10/30
 *
 */
#include <Arduino.h>
/*==========================================================================================
	内部定数定義
==========================================================================================*/

// #define OE      33
#define LATCH   25 
#define SDI     26
#define SCK     27

// #define LATCH   2 
// #define SDI     17
// #define SCK     5
/*==========================================================================================
	内部変数定義
==========================================================================================*/

hw_timer_t * timer = NULL;


const byte ascii[128] = {                                            
    /*   |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  | */
    /*   |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | */
    /*0*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*   |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     | */
    /*1*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*   |     |     |     |     |     |     |     |     |     |     |     |     |     |  -  |     |     | */
    /*2*/ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
    /*   |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |     |     |     |     |     |     | */
    /*3*/ 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xF6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
    /*   |     |  A  |     |  C  |     |  E  |  F  |  G  |  H  |     |  J  |     |  L  |     |     |     | */
    /*4*/ 0x00, 0xEE, 0x00, 0x9C, 0x00, 0x9E, 0x8E, 0x00, 0x6E, 0x00, 0x78, 0x00, 0x1C, 0x00, 0x00, 0x00, 
    /*   |  P  |     |     |  S  |  T  |  U  |     |     |     |     |  Z  |     |     |     |     |     | */
    /*5*/ 0xCE, 0x00, 0x00, 0xB6, 0x8C, 0x7C, 0x00, 0x00, 0x00, 0x00, 0xDA, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*   |     |     |  b  |  c  |  d  |     |     |     |     |  i  |     |     |     |     |  n  |  o  | */
    /*6*/ 0x00, 0x00, 0x3E, 0x1A, 0x7A, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x3A,
    /*   |     |     |  r  |     |     |     |  v  |     |     |  y  |     |     |     |     |     |     | */
    /*7*/ 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// static char dispData[] = {"    "}; //セグメント表示データ
// static byte bitData[] = {0,0,0,0}; //表示ビットデータ
// static byte dispLed = 0b0000;      //LEDデータ 0:OFF 1:ON
// static char dispFloat = 0;         //小数点位置(桁表示操作)0~4
// static byte dispDot = 0b0000;      //ドット表示0~4

/**
 * @struct	LodingData
 * @brief	ローディングデータ
 */
typedef struct lodingdata{
  char flag = 0;            //フラグ　0：無効　1：有効
  unsigned long time = 0;   //更新時間
  char seg;                 //処理桁　0~3:桁(bitDataの配列を指定)
  char backBitData;         //ローディング前の表示保管
}LodingData;

/**
 * @struct	FlashData
 * @brief	点滅データ
 */
typedef struct flashdata{
  char flag = 0;            //フラグ　0：無効　1：有効
  unsigned long time = 0;   //更新時間
  char seg;                 //処理桁 0~3:桁(bitDataの配列を指定)
  char backBitData;         //点滅前の表示保管
}FlashData;

/**
 * @struct	AverageData
 * @brief	平均化データ
 */
typedef struct dispdata{
  char strData[5];          //セグメント表示データ
  char bitData[4];          //表示ビットデータ
  char ledData = 0b0000;    //LEDデータ 0:OFF 1:ON
  char floatSeg = 0;        //小数点位置(桁表示操作)4:OFF 0~3:桁(bitDataの配列を指定)
  char dotData = 0b0000;    //ドット表示 0:OFF 1:ON
  LodingData loding;        //ローディング処理データ
  FlashData flash;          //点滅処理データ
}DispData;

DispData disp;
/*==========================================================================================
	内部関数定義
==========================================================================================*/

void hc595(int a, int b);
void IRAM_ATTR dispMain();
void lodingDisp(void);
/**
 *	@brief		セグメント表示 初期化
 *	@param		なし
 *	@retval		なし
 */
void dispInit() {
  pinMode(LATCH , OUTPUT);
  pinMode(SCK   , OUTPUT);
  pinMode(SDI   , OUTPUT);
  digitalWrite(LATCH, LOW);
  digitalWrite(SCK  , LOW);
  digitalWrite(SDI  , LOW);
  hc595(0x00,0x00);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &dispMain, true);
  timerAlarmWrite(timer, 500, true);
  timerAlarmEnable(timer);  
}


/**
 *	@brief		シフトレジスタ制御
 *	@param		a:上位バイト,b:下位バイト
 *	@retval		なし
 */
void hc595(int a, int b){
  shiftOut(SDI,SCK,LSBFIRST,b);
  shiftOut(SDI,SCK,LSBFIRST,a);
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
}

/**
 *	@brief		セグメント表示メイン処理(500μs毎割込み)
 *	@param		なし
 *	@retval		なし
 */
void IRAM_ATTR dispMain(){
  static int count = 0; //桁カウント
  byte upByte = 0;  //シフトレジスタ上位バイト
  byte downByte = 0;//シフトレジスタ下位バイト

  //ローディング表示
  if(disp.loding.flag){
    lodingDisp();
  }
  //点滅表示
  if(disp.flash.flag){
    
  }
  if(count < 4){
    upByte = disp.bitData[count];  //指定桁のアスキーデータ
    downByte = 0x01 << count; //指定桁のCOM出力
    downByte |= (disp.ledData<<4); //LED表現を追加
    
    hc595(upByte, downByte);  //シフトレジスタへ出力
    count++;  //次の桁へ
  }else{
    //全桁表示したら全て消灯してリセット
    hc595(upByte, downByte);
    count=0;
  }
}

/**
 *	@brief		表示データバイト変換
 *	@param		str:表示文字　typ　0：数字　1：文字
 *	@retval		なし
 */
void strToByt(char *str, char typ){
  int count = 0;
  for(count=0; count<4; count++){
    disp.bitData[count] = ascii[str[count]];  //指定桁のアスキーデータ
    if(((!typ)&&(count == disp.floatSeg))||
       (disp.dotData&(0x08>>count))){   //指定した場所に小数点,ドットを追加
      disp.bitData[count] |= 0x01;
    }else{
      disp.bitData[count] &= 0xFE;
    }
  }
  disp.flash.backBitData = disp.bitData[disp.flash.seg]; 
  disp.loding.backBitData = disp.bitData[disp.loding.seg]; 
}
/**
 *	@brief		数字表示データ更新
 *	@param		seg:セグメントデータ
 *	@retval		なし
 */
void setNumDisp(int seg){
  int i = 0;
  int sign = 0;
  //9999以上の場合は9999を表示
  if(seg >= 9999){
    seg = 9999;
  }
  //マイナスの場合は符号をつけて-999まで表現
  if(seg < 0){
    sign = 1; //符号フラグ
    seg *= -1;  //絶対値へ
    if(seg >= 999){
      seg = 999;
    }
  }
  //桁分け
  while(seg){
    disp.strData[3-i] = seg % 10;
    disp.strData[3-i] += '0';
    seg /= 10;
    i++;
  }
  //黒塗り表現
  for(i; i < 4; i++){
    if(i < (4-disp.floatSeg)){  //少数点までは0を表示（桁位置は修正）
      disp.strData[3-i] = '0';
    }else{
      if(sign){ //マイナスの場合は数字以降で表現
        disp.strData[3-i] = '-';
        sign = 0;
      }else{  //黒塗り
        disp.strData[3-i] = ' ';
      }
    }
  }
  strToByt(disp.strData,0);
}

/**
 *	@brief		LED更新
 *	@param		0:OFF 1:ON
 *	@retval		なし
 */
void setLed(char led){
  if(led <= 8){
    disp.ledData = led;
  }
}

/**
 *	@brief		LED状態取得
 *	@param		なし
 *	@retval		0:OFF 1:ON
 */
char getLed(){
  return disp.ledData;
}
/**
 *	@brief		文字表示データ更新
 *	@param		seg:セグメントデータ
 *	@retval		なし
 */
void setStrDisp(const char *str){
  int i = 0;
  for(i=0;i<4;i++){
    if(*str == '\0'){
      for(i;i<4;i++){
        disp.strData[i] = ' ';
      }
      break;
    } 
    disp.strData[i] = *str;
    str++;
  }
  strToByt(disp.strData,1);
  
}

/**
 *	@brief		小数点位置設定
 *	@param		0：無し　1～4：小数点桁
 *	@retval		なし
 */
void setFloatDisp(unsigned char f){
  //5以上では0にする
  if(f <= 4){
   f = 4 - f; //桁位置に合わして反転
  }else{
    f = 4;    //表示配列は0～3までなので無視される。
  }
  disp.floatSeg = f; 
}

/**
 *	@brief		ドット位置設定
 *	@param		0:OFF 1:ON
 *	@retval		なし
 */
void setDotDisp(int a,int b,int c, int d){
  byte buff = 0;
  if(a) buff =  8;
  if(b) buff += 4;
  if(c) buff += 2;
  if(d) buff += 1;
  disp.dotData = buff;
}

/**
 *	@brief		ローディング表示処理(表示処理の割込み内から呼び出し)
 *	@param		なし
 *	@retval		なし
 */
void lodingDisp(void){
  static unsigned long backTime = millis();
  static byte lodingLog = 0b00001100;
  //上書きがあった場合は最初から開始
  if(lodingLog != disp.bitData[disp.loding.seg]){ 
    backTime = millis();
    lodingLog = 0b00001100;
  }
  if((millis() - backTime) > disp.loding.time){
    backTime = millis();
    if(lodingLog & 0b00000100){
      lodingLog = lodingLog >> 1;
      lodingLog |= 0b10000000;
      lodingLog &= 0b11111100;
    }else{
      lodingLog = lodingLog >> 1;
    }
  }
  disp.bitData[disp.loding.seg] = lodingLog;
}

/**
 *	@brief		ローディング中表示開始
 *	@param		seg 1～4：処理桁　time 点滅周期(ms)
 *	@retval		なし
 */
void startLodingDisp(unsigned int seg, unsigned long time){
  //ローディング無効かつ設定桁が1～4の場合のみ処理
  if((!disp.loding.flag)&&(seg <= 4)&&(seg)){ 
    seg = 4 - seg; //桁位置に合わして反転
    disp.loding.flag = 1;
    disp.loding.seg = seg;
    disp.loding.time = time;
  }
}
/**
 *	@brief		ローディング中表示終了
 *	@param		なし
 *	@retval		なし
 */
void endLodingDisp(void){
  //ローディング中のみ処理
  if(disp.loding.flag){
    disp.loding.flag = 0;
    disp.bitData[disp.loding.seg] = disp.loding.backBitData;  //ローディング前のデータに戻す
  }
}