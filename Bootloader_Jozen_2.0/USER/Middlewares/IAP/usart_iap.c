#include "usart_iap.h"

/*-----------------------------------------------�ڲ�����------------------------------------------------------*/

//���ճ�����
static uint32_t Progress_Receive(void)
{
	/*--------------------------ָ�-------------------------*/
	char rec[] = "���ݽ������\r\n";
	char trans[] = "��ʹ�ô��ڷ���.bin�ļ�!\r\n";	
	char error[] = "������������,��ѡ����.bin�ļ�\r\n";
	/*--------------------------------------------------------*/
	uint16_t oldcount=0;							//�ϵĴ��ڽ�������ֵ
	uint32_t applenth=0;				    	//���յ���app���볤��
	HAL_UART_Transmit(&huart1,(uint8_t*)trans,sizeof(trans),MAX_WAIT_TIME);
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)	//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				if(USART_RX_CNT > 50)	//��ֹ�����
				{
					applenth=USART_RX_CNT;
					oldcount=0;
					USART_RX_CNT=0;
					HAL_UART_Transmit(&huart1,(uint8_t*)rec,sizeof(rec),MAX_WAIT_TIME);	//���������ʾ
					return applenth;
				}
				else
				{
					USART_RX_CNT=0;
					HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);	//������ʾ
				}
			}
			else 
			{
				oldcount=USART_RX_CNT;			
			}
		}
		HAL_Delay(10);
	}	
}

//ִ�г�����
void Usart_Execute_Progress(void)
{
	char start[] = "��ʼִ�г���!\r\n";
	char error[] = "��������,�޷�ִ��!\r\n";
	
	if(((*(__IO uint32_t*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
	{	 
		HAL_UART_Transmit(&huart1,(uint8_t*)start,sizeof(start),MAX_WAIT_TIME);	//ִ�г�����ʾ
		iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
	}
	else
	{
		HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);	//ִ�г�����ʾ
	}
}

//�̼�����
void Usart_Hardware_Update(void)
{
	/*--------------------------ָ�-------------------------*/
	char update[] = "�̼��������!\r\n";
	/*--------------------------------------------------------*/
	uint32_t appsize;
	appsize = Progress_Receive();																								//���ճ���
	if(((*(__IO uint32_t*)(0X20001000+4))&0xFF000000)==0x08000000)							//�ж��Ƿ�Ϊ0X08XXXXXX.
	{	 
		iap_write_appbin(FLASH_APP1_ADDR,Progress_Buff,appsize);									//�����ڲ�FLASH����   
		HAL_UART_Transmit(&huart1,(uint8_t*)update,sizeof(update),MAX_WAIT_TIME);	//���͸��������ʾ	
	}
}

/*------------------------------------------------API����-----------------------------------------------------*/

//����: 0 �̼�����		1 ִ�г���
void Usart_Interaction(uint8_t flag)
{
	if(flag == 0) Usart_Hardware_Update();
	else Usart_Execute_Progress();
}
/*-------------------------------------------------------------------------------------------------------------*/
