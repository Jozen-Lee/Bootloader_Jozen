#ifndef __INTERACTION_H_
#define __INTERACTION_H_
#include "main.h"
#include "usart.h"

/*--------------------程序接收模式选择-------------------------*/
#define USART_MODE 					1				//串口接收模式
#define SDCARD_MODE 				0				//SD卡接收模式
#define EXTERNAL_FLASH_MODE	0				//外部FLASH接收模式
/*-----------------------------------------------------------*/

/*--------------------内部静态函数----------------------------*/
static uint8_t Inquiry_Usart(void);																//指令收发
static void Hardware_Update(uint8_t *appbuf,uint32_t appsize);		//固件更新
static uint32_t Progress_Receive(void);														//APP接收
static void Execute_Progress(void);																//APP执行
/*-----------------------------------------------------------*/

/*-----------------------API函数-----------------------------*/
void Interaction_Usart(void);																			//串口交互
/*-----------------------------------------------------------*/

#endif
