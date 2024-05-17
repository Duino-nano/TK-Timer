/**
 *
 *	@file		average.hpp
 *
 *	@brief		平均化処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/11/09
 *
 */
#ifndef  __AVREAGE_HPP__
#define __AVREAGE_HPP__
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

/*==========================================================================================
	関数定義
==========================================================================================*/
/**
 * @class 	RingAverage
 * @brief	移動平均クラス
 */
class RingAverage{
	private:
		unsigned int    count;              //カウント数
		long            averagelog;        //平均過去データ
		long            *box;              //データ保存配列
		unsigned int    data_n;             //平均化N数
		int             initFlag;           //初期化フラグ
		unsigned  int   updataFlagCount;    //データ更新フラグ
		
		int             changeRate(long);   //変化量取得

	public:
		RingAverage(int);    //平均化処理設定
		~RingAverage(void);  //デストラクタ
		void    add(long);      //データ追加
		long    get(void);      //データ取得
		int     isUpdata(void); //全ての配列のデータが満たされたか確認 
		void    init(void);     //データ初期化
};

/**
 * @class 	Average
 * @brief	単純平均クラス
 */
class Average{
	private:
        int     count;      //カウント数
        long    total;      //合計データ
        long    max;        //最大データ
        long    min;        //最小データ
        int     data_n;             //平均化N数
        int     initFlag;           //初期化フラグ

	public:
        void    bigin(int);             //平滑化設定
        int     main(long,long*);    //データ追加
        void    init(void);             //データ初期化
};
#endif