/**
 *
 *	@file		can.hpp
 *
 *	@brief		can処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/11/09
 *
 */
#ifndef  __CAN_HPP__
#define __CAN_HPP__
/*==========================================================================================
	定数定義
==========================================================================================*/

/**
 * @enum	eCanAddress
 * @brief	CANアドレス
 */
typedef enum{
	// 車体情報
	eMM_CURRENT 	= 0x21,		//0x21 車体電流情報
	eMM_SPEED 		= 0x23,		//0x23 車体速度情報
	eMM_DIR  			= 0x25,		//0x25 車体方向情報
	eMM_BATTERY 	= 0x41,		//0x41 車体バッテリー電圧情報
	eMM_STEERING	= 0x81,		//0x81 車体ステアリング情報
	eMM_MODE 			= 0xA0,		//0xA0 車体モード
	eMM_ACCEL 		= 0xA1,		//0xA1 車体アクセル
	eMM_COUNTER 	= 0xA5,		//0xA5 車体カウンタ

	// BASIC情報
	eBASIC_STATE = 0x7BC,	//0x7BC 動作状態
	eBASIC_STOP,					//0x7BD 停止信号
	eBASIC_ACCEL,					//0x7BE アクセル

	// WORK情報
	eWORK_STOP 	= 0x7CC,	//0x7CC WORK停止信号
	eWORK_OUT,						//0x7CD WORK出力
	eWORK_INFO,						//0x7CE WORK情報

	// Sensor情報
	eSAFE_SONAR 	= 0x7DC,	//0x7DC ソナー情報

	// AutoDrive情報
	eDRIVE_INFO 	= 0x7EC,	//0x7EC 自動走行情報
	eDRIVE_DATA1,						//0x7ED 自動走行情報
	eDRIVE_DATA2,						//0x7EE 自動走行情報
}eCanAddress;
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
extern void canBegin();
extern void canIsSend();

#endif