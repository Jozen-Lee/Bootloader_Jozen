#ifndef __IAP_H__
#define __IAP_H__
#include "main.h"
#include "usart.h"
#include "stmflash.h"


#define FLASH_APP1_ADDR		0x08010000  							//第一个应用程序起始地址(存放在FLASH),保留0X08000000~0X0800FFFF的空间为Bootloader使用(共64KB)	
#define PRO_MAX_NUM				10												//程序存储的最大数目
#define PRO_SIZE					179*1024									//程序最大内存
#define NAME_SIZE					1*1024										//程序名最大内存
#define PRO_MAX_SIZE 			(PRO_SIZE+NAME_SIZE)			//程序最大占据的内存,包括程序名以及程序本身,括号不能省!



extern uint8_t Progress_Buff[PRO_MAX_SIZE];
typedef  void (*iapfun)(void);				//定义一个函数类型的参数.   
void iap_load_app(uint32_t appxaddr);			//跳转到APP程序执行
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t applen);	//在指定地址开始,写入bin
#endif







































