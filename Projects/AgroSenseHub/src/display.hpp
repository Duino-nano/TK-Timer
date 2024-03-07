/**
 *
 *	@file		display.hpp
 *
 *	@brief		RFID処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/10/30
 *
 */
#ifndef  __DISPLAY_HPP__
#define __DISPLAY_HPP__
/*==========================================================================================
	定数定義
==========================================================================================*/


/*==========================================================================================
	変数定義
==========================================================================================*/
/**
 * @struct	RackInfo
 * @brief	ラック設定
 */

/*==========================================================================================
	関数定義
==========================================================================================*/
extern void dispInit();
extern void setDotDisp(int a,int b,int c, int d);
extern void setFloatDisp(unsigned char f);
extern void setStrDisp(const char *str);
extern void setLed(char);
extern char getLed();
extern void setNumDisp(int seg);
extern void startLodingDisp(unsigned int seg, unsigned long time);
extern void endLodingDisp(void);
#endif