/**
 *
 *	@file		average.cpp
 *
 *	@brief		平均化処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/11/09
 *
 */
#include <Arduino.h>
#include "avreage.hpp"
#include <limits.h>
/*==========================================================================================
	外部関数定義
==========================================================================================*/


/*==========================================================================================
	内部定数定義
==========================================================================================*/

/*==========================================================================================
	内部関数定義
==========================================================================================*/
/*==========================================================================================
	外部変数定義
==========================================================================================*/
/*==========================================================================================
	内部変数定義
==========================================================================================*/
/**
 *	@var		
 *	@brief		
 */
/*==========================================================================================
	関数
==========================================================================================*/

/**
 *	@brief		RingAverage 平均化処理設定
 *	@param		n:平均化数　rate:一括更新変化量
 *	@retval		なし
 */
RingAverage::RingAverage(int n, int rate){
    box = new long[n];  //指定量の配列を確保
    changeRate = rate;
    data_n = n;
    init();         
}

/**
 *	@brief		RingAverage デストラクタ処理
 *	@param		なし
 *	@retval		なし
 */
RingAverage::~RingAverage(void){
    delete[] box; // メモリの解放
}

/**
 *	@brief		RingAverage データ追加処理
 *	@param		data:生データ
 *	@retval		なし
 */
void RingAverage::add(long data){
    int i = 0;

    //変化量が設定値以上の場合は初期化
    if(getChangeRate(data) > changeRate){
        init();
    }
    //初期化後は配列を更新
    if(initFlag){
        initFlag = 0;
        averagelog = data;
        for(i = 0; i < data_n; i++){
            box[i] = data;
        }
    }
    
    box[count] = data;  //配列にデータを追加
    if(count < data_n){ //次のアドレスへ
        count++;
    }else{
        count = 0; 
    }

    if(updataFlagCount<UINT16_MAX){ //オーバーフロ－まで更新回数をカウント
        updataFlagCount++;
    }
}
/**
 *	@brief		RingAverage 平均データ取得
 *	@param		なし
 *	@retval		total:移動平均値
 */
long RingAverage::get(void){
    int i = 0;
    long max = box[i];          //最大
    long min = box[i];          //最小
    long total = 0;             //合計値
    //配列データの合計、最大最小を取得
    for(i = 0; i < data_n; i++){
        total += box[i];
        if(box[i] < min){
            min = box[i];
        }
        if(box[i] > max){
            max = box[i];
        }
    }
    //平均データn数が2より多い場合は最大最小を除き平均化
    if(data_n <= 2){
        total /= (long)data_n;
    }else{
        total -= (max + min);
        total /= (long)(data_n-2); 
    }
    averagelog = abs(total);
    updataFlagCount = 0;    //取得したタイミングで更新をリセット
    return total;
}

/**
 *	@brief		RingAverage 初期化処理
 *	@param		なし
 *	@retval		なし
 */
void RingAverage::init(void){
    count = 0;            //カウント数
    initFlag = 1;
    updataFlagCount = 0;
}

/**
 *	@brief		RingAverage 全データ更新確認
 *	@param		なし
 *	@retval		0：未更新　1：更新済
 */
int RingAverage::isUpdata(void){
    int result = 0;
    if(updataFlagCount >= data_n){  //全データ分更新されたか？
        result = 1;
    }
    return result;
}

/**
 *	@brief		RingAverage データ変化量取得
 *	@param		次データ
 *	@retval		rate:変化率*0.01%
 */
int RingAverage::getChangeRate(long data){
    long rate = 0;
    //平均値1以上で処理をする
    if(averagelog){
        if(data != averagelog){//変化量が1以上あることを確認
            rate = labs(labs(data) - averagelog);
            rate *= 10000;
            rate /= averagelog;
        }
    }
    return (int)rate;
}

/**
 *	@brief		Average 初期設定
 *	@param		n平均化数 （3以上にすること）
 *	@retval		無し
 */
void Average::bigin(int n){
    if(n < 3){      //設定値確認(3以上)
        data_n = 3;
    }
    data_n = n;
    init();
}

/**
 *	@brief		Average 初期化
 *	@param		無し
 *	@retval		無し
 */
void Average::init(void){
    count = 0;   
    total = 0;   
    max = 0;     
    min = 0;     
    initFlag = 0;
}

/**
 *	@brief		Average メイン処理
 *	@param		data:生データ　average：平均値格納先アドレス
 *	@retval		result　0：平均化中　1：平均化完了
 */
int Average::main(long data, long *average){
    int result = 0;
    if(!count){     //初期は最大最小に最初の値を与える
        max = data;
        min = data;
    }
    if(count < data_n){
        count++;
        total += data;
        if(data > max){
            max = data;
        }
        if(data < min){
            min = data;
        }
    }

    if(count >= data_n){    //データがそろったら平均化
        total -= (min + max);
        total /= (count - 2);
        *average = total;
        init();
        result = 1;
    }
    return result;
}