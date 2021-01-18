#ifndef __FLASH_IAP_H_
#define __FLASH_IAP_H_

#include "iap.h"
#include "flash.h"
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
