#ifndef __INTERACTION_H_
#define __INTERACTION_H_
#include "main.h"
#include "flash_iap.h"
#include "usart_iap.h"
#include "SDcard_iap.h"

/*--------------------程序接收模式选择-------------------------*/
#define USART_MODE					0				//仅串口模式
#define SDCARD_MODE 				1				//SD卡模式
#define FLASH_MODE					0				//外部FLASH模式
/*-----------------------------------------------------------*/

/*--------------------内部静态函数----------------------------*/
static uint8_t Inquiry(void);	//指令收发
/*-----------------------------------------------------------*/

/*-----------------------API函数-----------------------------*/
void Interaction(void);	//用户交互
/*-----------------------------------------------------------*/

#endif
