#ifndef __INTERACTION_H_
#define __INTERACTION_H_
#include "main.h"
#include "usart.h"

/*--------------------�������ģʽѡ��-------------------------*/
#define USART_MODE 					1				//���ڽ���ģʽ
#define SDCARD_MODE 				0				//SD������ģʽ
#define EXTERNAL_FLASH_MODE	0				//�ⲿFLASH����ģʽ
/*-----------------------------------------------------------*/

/*--------------------�ڲ���̬����----------------------------*/
static uint8_t Inquiry_Usart(void);																//ָ���շ�
static void Hardware_Update(uint8_t *appbuf,uint32_t appsize);		//�̼�����
static uint32_t Progress_Receive(void);														//APP����
static void Execute_Progress(void);																//APPִ��
/*-----------------------------------------------------------*/

/*-----------------------API����-----------------------------*/
void Interaction_Usart(void);																			//���ڽ���
/*-----------------------------------------------------------*/

#endif
