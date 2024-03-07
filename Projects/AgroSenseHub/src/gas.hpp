/**
 *
 *	@file		gas.hpp
 *
 *	@brief		gas処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/12/25
 *
 */
#ifndef  __GAS_HPP__
#define __GAS_HPP__
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
 * @class 	IntervalTimer
 * @brief	Timer処理
 */
extern int gasStatus;
/*==========================================================================================
	関数定義
==========================================================================================*/
extern int wifiConect();
extern void accessToGoogleSheets(long *data, int size);
#endif