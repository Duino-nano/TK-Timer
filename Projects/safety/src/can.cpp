/**
 *
 *	@file		can.cpp
 *
 *	@brief		CAN通信処理
 *
 *	@author		株式会社エムスクエア・ラボ　山形
 *
 *	@date		create : 2023/11/09
 *
 */
#include <Arduino.h>
#include "driver/gpio.h"
#include "driver/twai.h"
#include "sonar.hpp"
#include "can.hpp"
#define TESTPIN 21
/*==========================================================================================
	外部関数定義
==========================================================================================*/


/*==========================================================================================
	内部定数定義
==========================================================================================*/

/*CAN設定*/
#define CAN_TX GPIO_NUM_27
#define CAN_RX GPIO_NUM_26
/*==========================================================================================
	内部関数定義
==========================================================================================*/
void canIsReceive(void *args);
/*==========================================================================================
	外部変数定義
==========================================================================================*/
/*==========================================================================================
	内部変数定義
==========================================================================================*/

twai_message_t message_D0; // CANメッセージ
twai_message_t message_D1; // CANメッセージ
uint8_t dataD0[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // CANメッセージのデータ部
uint8_t dataD1[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // CANメッセージのデータ部
TaskHandle_t thp[1]; // タスクハンドル

/*==========================================================================================
	関数
==========================================================================================*/

/**
 *	@brief		CAN初期設定
 *	@param		無し
 *	@retval		無し
 */
void canBegin(){
  pinMode(TESTPIN, OUTPUT);
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX, CAN_RX, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  //Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
      printf("Driver installed\n");
  } else {
      printf("Failed to install driver\n");
      return;
  }

  //Start TWAI driver
  if (twai_start() == ESP_OK) {
      printf("Driver started\n");
  } else {
      printf("Failed to start driver\n");
      return;
  }
  message_D0.identifier = eSAFE_SONAR;		//送信データアドレス設定
  message_D0.data_length_code = 8;	//送信データ長
  
  for (int i = 0; i < 8; i++) {
    message_D0.data[i] = dataD0[i];
  }
  // xTaskCreatePinnedToCore(canIsReceive, "cmd", 4096, NULL, 24, &thp[0], 0);
}

/**
 *	@brief		CAN送信処理
 *	@param		無し
 *	@retval		無し
 */
void canIsSend(){
	  dataD0[0] = (sonarData[0] >> 8) & 0xFF;
    dataD0[1] = sonarData[0] & 0xFF;
    dataD0[2] = (sonarData[1] >> 8) & 0xFF;
    dataD0[3] = sonarData[1] & 0xFF;
    dataD0[4] = (sonarData[2] >> 8) & 0xFF;
    dataD0[5] = sonarData[2] & 0xFF;
    dataD0[6] = (sonarData[3] >> 8) & 0xFF;
    dataD0[7] = sonarData[3] & 0xFF;
    for (int i = 0; i < 8; i++) {
      message_D0.data[i] = dataD0[i];
    }
    twai_transmit(&message_D0, pdMS_TO_TICKS(50));
}

/**
 *	@brief		CAN受信処理
 *	@param		無し
 *	@retval		無し
 */
void canIsReceive(void *args) {
  // while (1) {
  //   twai_message_t message;
  //   if (twai_receive(&message, pdMS_TO_TICKS(100)) == ESP_OK) {
  //     if(message.identifier == 0x20){	//WORK命令アドレスまで待機
	// 	//WORK1chの制御
  //   digitalWrite(TESTPIN, message.data[0]); //CAN更新時間確認
  //       work.out[0].state = message.data[0];
  //   //WORK2chの制御
  //       work.out[1].state = message.data[1];
  //     }
  //   }
  // }
}