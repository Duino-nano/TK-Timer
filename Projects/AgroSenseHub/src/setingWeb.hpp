/**
 *
 *	@file		setingWeb.cpp
 *
 *	@brief		設定用webアプリ
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/11/09
 *
 */
#ifndef  __SETINGWEB_HPP__
#define __SETINGWEB_HPP__

#include <ESPAsyncWebServer.h>
#include "key.hpp"
/*==========================================================================================
	定数定義
==========================================================================================*/


/*==========================================================================================
	変数定義
==========================================================================================*/
/**
 * @struct	GasConfig
 * @brief	GAS設定データ
 */
typedef struct gasconfig{
    char devId[128];
    int  interval;
}GasConfig;

/**
 * @struct	WifiConfig
 * @brief	wifi設定データ
 */
typedef struct wificonfig{ 
    char ssid[128];
    char ps[128];
}WifiConfig;

typedef enum setflag{ 
    eINITIAL = -1,
    eSETING,
    eCOMPLETE
}Setflag;
/**
 * @struct	ConfigData
 * @brief	設定データ
 */
typedef struct configdata{
    GasConfig gas;
    WifiConfig wifi;
    Setflag flag = eINITIAL;
}ConfigData;

extern ConfigData config;
/*==========================================================================================
	関数定義
==========================================================================================*/
extern void setingWeb();
extern void setWifiMode(KeyInput &key);
#endif