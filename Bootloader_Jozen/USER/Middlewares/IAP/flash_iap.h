#ifndef __FLASH_IAP_H_
#define __FLASH_IAP_H_

#include "iap.h"
#include "flash.h"

/*------------------------------存储地址---------------------------------------*/
//每个程序占179K,程序名占1K,即整个程序占180K
#define FLASH_PROGRESS_1 			0
#define FLASH_NAME_1					(PRO_MAX_SIZE*1)-1024
#define FLASH_PROGRESS_2 		 	(PRO_MAX_SIZE*1)
#define FLASH_NAME_2					(PRO_MAX_SIZE*2)-1024
#define FLASH_PROGRESS_3 		 	(PRO_MAX_SIZE*2)
#define FLASH_NAME_3					(PRO_MAX_SIZE*3)-1024
#define FLASH_PROGRESS_4 		 	(PRO_MAX_SIZE*3)
#define FLASH_NAME_4					(PRO_MAX_SIZE*4)-1024
#define FLASH_PROGRESS_5 		 	(PRO_MAX_SIZE*4)
#define FLASH_NAME_5					(PRO_MAX_SIZE*5)-1024
#define FLASH_PROGRESS_6 		 	(PRO_MAX_SIZE*5)
#define FLASH_NAME_6					(PRO_MAX_SIZE*6)-1024
#define FLASH_PROGRESS_7 		 	(PRO_MAX_SIZE*6)
#define FLASH_NAME_7					(PRO_MAX_SIZE*7)-1024
#define FLASH_PROGRESS_8 		 	(PRO_MAX_SIZE*7)
#define FLASH_NAME_8					(PRO_MAX_SIZE*8)-1024
#define FLASH_PROGRESS_9 		 	(PRO_MAX_SIZE*8)
#define FLASH_NAME_9					(PRO_MAX_SIZE*9)-1024
#define FLASH_PROGRESS_10 	 	(PRO_MAX_SIZE*9)
#define FLASH_NAME_10					(PRO_MAX_SIZE*10)-1024

/*-----------------------------------------辅助性函数------------------------------------------*/
static uint32_t Pro_Name_Address(uint8_t n);																							//返回对应的程序名存储地址
static uint32_t Pro_Address(uint8_t n);																										//返回对应的程序存储地址
static uint16_t Pro_Name_Length(char* name);																							//返回对应的程序名长度
static void MY_Write_Flash(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite);	//写FLASH函数,满足写入的程序过长时的写入FLASH操作
static void MY_Read_Flash(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead);			//读LASH函数,满足读取的程序过长时的读取FLASH操作
/*--------------------------------------------------------------------------------------------*/

/*-----------------------------------------内部函数--------------------------------------------*/
static void Receive_Name(void);																														//接收程序名
static void Clear_Buff(void);																															//清空程序缓存区,并将计数器置0
static void Read_Pro_Name_Flash(void);																										//从FLASH中读取程序名,并通过串口发送出去
static uint8_t Choose_Pro(uint8_t mode);																									//选择所要访问的程序序号
static void Flash_Hardware_Update(void);																									//固件更新函数
static void Flash_Execute_Progress(void);																									//执行程序函数
/*--------------------------------------------------------------------------------------------*/

/*-----------------------------------------API函数--------------------------------------------*/
void Flash_Interaction(uint8_t flag);
/*--------------------------------------------------------------------------------------------*/


#endif
