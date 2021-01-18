#ifndef __FLASH_IAP_H_
#define __FLASH_IAP_H_

#include "iap.h"
#include "flash.h"
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
