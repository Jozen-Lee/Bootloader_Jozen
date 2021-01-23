#ifndef __INTERACTION_H_
#define __INTERACTION_H_
#include "main.h"
#include "flash_iap.h"
#include "usart_iap.h"
#include "SDcard_iap.h"

/*--------------------�������ģʽѡ��-------------------------*/
#define USART_MODE					0				//������ģʽ
#define SDCARD_MODE 				1				//SD��ģʽ
#define FLASH_MODE					0				//�ⲿFLASHģʽ
/*-----------------------------------------------------------*/

/*--------------------�ڲ���̬����----------------------------*/
static uint8_t Inquiry(void);	//ָ���շ�
/*-----------------------------------------------------------*/

/*-----------------------API����-----------------------------*/
void Interaction(void);	//�û�����
/*-----------------------------------------------------------*/

#endif
