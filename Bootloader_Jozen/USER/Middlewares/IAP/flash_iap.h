#ifndef __FLASH_IAP_H_
#define __FLASH_IAP_H_

#include "iap.h"
#include "flash.h"

/*------------------------------�洢��ַ---------------------------------------*/
//ÿ������ռ179K,������ռ1K,����������ռ180K
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

/*-----------------------------------------�����Ժ���------------------------------------------*/
static uint32_t Pro_Name_Address(uint8_t n);																							//���ض�Ӧ�ĳ������洢��ַ
static uint32_t Pro_Address(uint8_t n);																										//���ض�Ӧ�ĳ���洢��ַ
static uint16_t Pro_Name_Length(char* name);																							//���ض�Ӧ�ĳ���������
static void MY_Write_Flash(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite);	//дFLASH����,����д��ĳ������ʱ��д��FLASH����
static void MY_Read_Flash(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead);			//��LASH����,�����ȡ�ĳ������ʱ�Ķ�ȡFLASH����
/*--------------------------------------------------------------------------------------------*/

/*-----------------------------------------�ڲ�����--------------------------------------------*/
static void Receive_Name(void);																														//���ճ�����
static void Clear_Buff(void);																															//��ճ��򻺴���,������������0
static void Read_Pro_Name_Flash(void);																										//��FLASH�ж�ȡ������,��ͨ�����ڷ��ͳ�ȥ
static uint8_t Choose_Pro(uint8_t mode);																									//ѡ����Ҫ���ʵĳ������
static void Flash_Hardware_Update(void);																									//�̼����º���
static void Flash_Execute_Progress(void);																									//ִ�г�����
/*--------------------------------------------------------------------------------------------*/

/*-----------------------------------------API����--------------------------------------------*/
void Flash_Interaction(uint8_t flag);
/*--------------------------------------------------------------------------------------------*/


#endif
