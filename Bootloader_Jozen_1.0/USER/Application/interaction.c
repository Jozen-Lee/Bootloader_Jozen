#include "interaction.h"

/*----------------------------���¶����ӳٺ���--------------------------*/
void HAL_Delay(uint32_t Delay)
{
	int i, j;
	for(i = 0; i < Delay; i++)
	{
		for(j = 0; j < 168000; j++);
	}
}

/*-----------------------��������-------------------------*/




//ѯ�������
//����ֵ: 0 ���¹̼� 1 ��������
static uint8_t Inquiry_Usart(void)
{
	char inquiry[] = "����������\r\n0:���¹̼�\r\n1.��������\r\n";
	char error[] = "������������,����������\r\n0:���¹̼�\r\n1.��������\r\n";
	HAL_UART_Transmit(&huart1,(uint8_t*)inquiry,sizeof(inquiry),MAX_WAIT_TIME);
	while(1)
	{
		if(USART_RX_CNT)
		{
			USART_RX_CNT = 0;	//���������
			if(USART_RX_BUF[0] == '0') return 0;																				//���¹̼�
			else if(USART_RX_BUF[0] == '1') return 1;																		//��������
			else HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);//������ʾ
		}
		HAL_Delay(10);		
	}
}

//�̼�����
static void Hardware_Update(uint8_t *appbuf,uint32_t appsize)
{
	char update[] = "�̼��������!\r\n";
	if(((*(__IO uint32_t*)(0X20001000+4))&0xFF000000)==0x08000000)							//�ж��Ƿ�Ϊ0X08XXXXXX.
	{	 
		iap_write_appbin(FLASH_APP1_ADDR,appbuf,appsize);										//����FLASH����   
		HAL_UART_Transmit(&huart1,(uint8_t*)update,sizeof(update),MAX_WAIT_TIME);	//���͸��������ʾ	
	}
}

//������պ���
//����ֵ: ���յĳ���Ĵ�С
static uint32_t Progress_Receive(void)
{
	char rec[] = "�û��������\r\n";
	
	/*-----------------���ڽ���------------------*/
	#ifdef USART_MODE
	uint16_t oldcount=0;						//�ϵĴ��ڽ�������ֵ
	uint32_t applenth=0;				    //���յ���app���볤��
	char trans[] = "��ʹ�ô��ڷ���.bin�ļ�!\r\n";	
	char error[] = "������������,��ѡ����.bin�ļ�\r\n";
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
	#endif
	
//	#ifdef SDCARD_MODE
//	
//	#endif
//	
//	#ifdef EXTERNAL_FLASH_MODE
//	
//	#endif
}

//ִ�г�����
static void Execute_Progress(void)
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

#ifdef USART_MODE
void Interaction_Usart(void)
{
	uint8_t flag;
	while(1)
	{
		//��������
		flag = Inquiry_Usart();
		
		//ִ�в���
		if(flag == 0) Hardware_Update(USART_RX_BUF, Progress_Receive());
		else Execute_Progress();
	}
}
#endif

