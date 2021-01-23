#ifndef __IAP_H__
#define __IAP_H__
#include "main.h"
#include "usart.h"
#include "stmflash.h"


#define FLASH_APP1_ADDR		0x08010000  							//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH),����0X08000000~0X0800FFFF�Ŀռ�ΪBootloaderʹ��(��64KB)	
#define PRO_MAX_NUM				10												//����洢�������Ŀ
#define PRO_SIZE					179*1024									//��������ڴ�
#define NAME_SIZE					1*1024										//����������ڴ�
#define PRO_MAX_SIZE 			(PRO_SIZE+NAME_SIZE)			//�������ռ�ݵ��ڴ�,�����������Լ�������,���Ų���ʡ!



extern uint8_t Progress_Buff[PRO_MAX_SIZE];
typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.   
void iap_load_app(uint32_t appxaddr);			//��ת��APP����ִ��
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t applen);	//��ָ����ַ��ʼ,д��bin
#endif







































