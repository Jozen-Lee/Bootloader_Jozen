#include "interaction.h"
#include "stdio.h"

//ѯ�������
//����ֵ: 0 ���¹̼� 1 ��������
static uint8_t Inquiry(void)
{
	char inquiry[] = "����������\r\n0:���¹̼�\r\n1.��������\r\n";
	char error[] = "������������,����������\r\n0:���¹̼�\r\n1.��������\r\n";
	HAL_UART_Transmit(&huart1,(uint8_t*)inquiry,sizeof(inquiry),MAX_WAIT_TIME);
	while(1)
	{
		if(USART_RX_CNT)
		{
			USART_RX_CNT = 0;	//���������
			if(Progress_Buff[0] == '0') return 0;																				//���¹̼�
			else if(Progress_Buff[0] == '1') return 1;																		//��������
			else HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);//������ʾ
		}
		HAL_Delay(10);		
	}
}

//���ô�����ʾ����
static void Confige_Error(void)
{
	char error[] = "Bootloader���ò�������,����Դ����!\r\n�˳�����!\r\n";
	HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);
}

//ͨ�����ڽ��н���
void Interaction(void)
{
	uint8_t flag;
	while(1)
	{
		//��������
		flag = Inquiry();
		
		//ִ�в���
		if(USART_MODE)			Usart_Interaction(flag);
		else if(FLASH_MODE) Flash_Interaction(flag);
//		else if(SDCARD_MODE) SDcard_Interaction(flag);
		else 
		{
			Confige_Error();	
			break;
		}
	}
}


