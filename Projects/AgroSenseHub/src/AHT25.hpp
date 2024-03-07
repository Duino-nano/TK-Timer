/**
 *
 *	@file		AHT25.hpp
 *
 *	@brief		温度湿度センサーAHT25処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2024/01/12
 *
 */
#ifndef  __AHT25_HPP__
#define __AHT25_HPP__
#include <CRC8.h>
#include "avreage.hpp"
#define ERROR_VALUE 99999
/*==========================================================================================
	定数定義
==========================================================================================*/


/*==========================================================================================
	変数定義
==========================================================================================*/
/**
 * @struct	
 * @brief	
 */
/**
 * @class 	AHT25
 * @brief	AHT25センサー処理
 */
class AHT25{
	private:
		long temperature;
		long humidity;
		unsigned long interval;
		int enablePin;
		RingAverage *tempAverage;
		RingAverage *humiAverage;
		CRC8 crc;

	public:
		AHT25(int pin, unsigned long t = 3000, int n = 20);    //インストラクタ
		~AHT25(void);		//デスインタラクタ
		int readData(void);      	//センサーデータ読み取り処理
		void printValues(void);     	//データプリント
		long getTemp(void);	//温度データ取得
		long getHumi(void);	//湿度データ取得
		void init(void);     	//データ初期化
};
/*==========================================================================================
	関数定義
==========================================================================================*/


#endif