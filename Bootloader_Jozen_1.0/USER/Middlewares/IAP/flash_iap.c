#include "flash_iap.h"
#include "stdio.h"

/*-----------------------------�����Ժ���-------------------------------*/
//���ض�Ӧ�ĳ������洢��ַ
//����:��������
static uint32_t Pro_Name_Address(uint8_t n)
{
	switch(n)
	{
		case 1: 	return FLASH_NAME_1;
		case 2: 	return FLASH_NAME_2;
		case 3: 	return FLASH_NAME_3;
		case 4: 	return FLASH_NAME_4;
		case 5: 	return FLASH_NAME_5;
		case 6: 	return FLASH_NAME_6;
		case 7: 	return FLASH_NAME_7;
		case 8: 	return FLASH_NAME_8;
		case 9: 	return FLASH_NAME_9;
		case 10: 	return FLASH_NAME_10;
	}
	return 0;
}

//���ض�Ӧ�ĳ���洢��ַ
//����:��������
static uint32_t Pro_Address(uint8_t n)
{
	switch(n)
	{
		case 1: 	return FLASH_PROGRESS_1;
		case 2: 	return FLASH_PROGRESS_2;
		case 3: 	return FLASH_PROGRESS_3;
		case 4: 	return FLASH_PROGRESS_4;
		case 5: 	return FLASH_PROGRESS_5;
		case 6: 	return FLASH_PROGRESS_6;
		case 7: 	return FLASH_PROGRESS_7;
		case 8: 	return FLASH_PROGRESS_8;
		case 9: 	return FLASH_PROGRESS_9;
		case 10: 	return FLASH_PROGRESS_10;
	}
	return 0;
}

//���ض�Ӧ�ĳ���������
//����:�洢������������ָ��
static uint16_t Pro_Name_Length(char* name)
{
	uint16_t count;
	for(uint16_t i=0; i< NAME_SIZE; i++)
	{
		if(name[i] != NULL) count++;
		else return count;
	}
	return 0;
}


//дFLASH����,����д��ĳ������ʱ��д��FLASH����
static void MY_Write_Flash(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite)
{
	uint8_t num;	//����Ҫ�ֶε���Ŀ
	uint8_t temp;
	/*�������������*/
	
	/*1.д������ݳ����㹻��*/
	if(NumByteToWrite <= 0xFFFF) SPI_Flash_Write(pBuffer, WriteAddr, NumByteToWrite);
	
	/*2.д������ݳ��ȹ���*/
	else
	{
		num = NumByteToWrite / 0xFFFF;
		for(temp = 0; temp < num; temp ++)
		{
			SPI_Flash_Write(pBuffer+temp*0xFFFF, WriteAddr+temp*0xFFFF, 0xFFFF);
		}
		SPI_Flash_Write(pBuffer+temp*0xFFFF, WriteAddr+temp*0xFFFF, NumByteToWrite%0xFFFF);
	}
}

//��LASH����,�����ȡ�ĳ������ʱ�Ķ�ȡFLASH����
static void MY_Read_Flash(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead)
{
	uint8_t num;	//����Ҫ�ֶε���Ŀ
	uint8_t temp;
	/*�������������*/
	
	/*1.д������ݳ����㹻��*/
	if(NumByteToRead <= 0xFFFF) SPI_Flash_Read(pBuffer, ReadAddr, NumByteToRead);
	
	/*2.д������ݳ��ȹ���*/
	else
	{
		num = NumByteToRead / 0xFFFF;
		for(temp = 0; temp < num; temp ++)
		{
			SPI_Flash_Write(pBuffer+temp*0xFFFF, ReadAddr+temp*0xFFFF, 0xFFFF);
		}
		SPI_Flash_Write(pBuffer+temp*0xFFFF, ReadAddr+temp*0xFFFF, NumByteToRead%0xFFFF);
	}
}

/*--------------------------------------------�ڲ�����-------------------------------------------------*/
//���ճ�����
static void Receive_Name(void)
{
	/*--------------------------ָ�-------------------------*/
	char input_name[] = "��Ϊ�����͵ĳ�������!\r\n";
	/*--------------------------------------------------------*/
	uint32_t oldcount;
	USART_RX_CNT = PRO_SIZE;				//�ڳ�������λ�ô洢������
	oldcount = USART_RX_CNT;
	HAL_UART_Transmit(&huart1,(uint8_t*)input_name,sizeof(input_name),MAX_WAIT_TIME);
	while(1)
	{
		if(USART_RX_CNT != oldcount) break;	//˵����ʼ��������
		HAL_Delay(10);
	}
	while(1)
	{
		if(oldcount==USART_RX_CNT)	//��������,û���յ��κ�����,��Ϊ�������ݽ������.
		{
			USART_RX_CNT = 0;
			break;
		}
		else 
		{
			oldcount=USART_RX_CNT;			
		}
		HAL_Delay(10);
	}		
}

//��ճ��򻺴���,������������0
static void Clear_Buff(void)
{
	USART_RX_CNT = 0;
	for(uint32_t i = 0; i < PRO_MAX_SIZE; i++)
	{
		Progress_Buff[i] = NULL;
	}
}

//��FLASH�ж�ȡ������,��ͨ�����ڷ��ͳ�ȥ
static void Read_Pro_Name_Flash(void)
{
	char name[NAME_SIZE];
	char num[] = "0. ";
	char empty[] = "��!\r\n";
	uint32_t add;					//�������洢��ַ
	uint16_t name_length;	//����������
	for(uint8_t i = 0; i < PRO_MAX_NUM; i++)
	{
		sprintf(num, "%d. ", i+1);
		add = Pro_Name_Address(i+1);																							//��ȡ����洢��ַ
		SPI_Flash_Read((uint8_t*)name, add, NAME_SIZE);														//��FLASH�ж�ȡ����
		name_length = Pro_Name_Length(name);																			//��ȡ����������
		name[name_length] = '\r'; name[name_length+1] = '\n';
		HAL_UART_Transmit(&huart1,(uint8_t*)num,sizeof(num),MAX_WAIT_TIME);				//��ӡ�������
		if(name[0] != NULL) 
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)name,name_length+2,MAX_WAIT_TIME);	//�г������ӡ������
		}	
		else 
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)empty,sizeof(empty),MAX_WAIT_TIME);	//û�г������ӡ��
		}
	}
}

////����FLASH����ѡ��ĳ���
//static void Clear_Pro_Flash(uint8_t num)
//{
//	uint32_t add_begin , add_end, temp;
//	add_begin = Pro_Address(num) / 4 / 1024;
//	add_end = Pro_Address(num+1) / 4 / 1024 - 1;
//	temp = add_begin;
//	while(temp <= add_end)
//	{
//		SPI_Flash_Erase_Sector(temp);
//		temp ++;
//	}
//}

//ѡ����Ҫ���ʵĳ������
//����ֵ:��Ҫ���ʵĳ������
//����: 0 д���ⲿFLASH, 1 д���ڲ�FLASH
static uint8_t Choose_Pro(uint8_t mode)
{
	uint16_t oldcount = 0;
	uint8_t choice = 0, temp = 0;
	char inquiry0[] = "��ѡ����Ҫ���ǵĳ������!\r\n";
	char inquiry1[] = "��ѡ����Ҫ���еĳ������!\r\n";
	if(mode == 0) HAL_UART_Transmit(&huart1,(uint8_t*)inquiry0,sizeof(inquiry0),MAX_WAIT_TIME);
	else HAL_UART_Transmit(&huart1,(uint8_t*)inquiry1,sizeof(inquiry1),MAX_WAIT_TIME);
	Read_Pro_Name_Flash();		//��ӡ������
	Clear_Buff();
	while(1)
	{
		if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)
			{
				while(Progress_Buff[temp]) 
				{
					choice = choice * 10 + (uint8_t)((char)Progress_Buff[temp] - '0');
					temp ++;
				}
				return choice;
			}
			else
			{
				oldcount = USART_RX_CNT;
			}
		}
		HAL_Delay(10);
	}
}

//�̼����º���
static void Flash_Hardware_Update(void)
{
	/*--------------------------ָ�-------------------------*/
	char update[] = "�̼��������!\r\n";
	char rec[] = "���ݽ������\r\n";
	char trans[] = "��ʹ�ô��ڷ���.bin�ļ�!\r\n";	
	char error[] = "������������,��ѡ����.bin�ļ�\r\n";
	/*--------------------------------------------------------*/
	uint16_t oldcount=0;							//�ϵĴ��ڽ�������ֵ
	uint32_t add;											//д��FLASH�ĵ�ַ
	add = Pro_Address(Choose_Pro(0));	//�����������˻�ȡ�������,��������Ż�ȡ��Ҫд��FLASH�ĵ�ַ
	HAL_UART_Transmit(&huart1,(uint8_t*)trans,sizeof(trans),MAX_WAIT_TIME);
	Clear_Buff();
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)	//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				if(USART_RX_CNT > 50)	//��ֹ�����
				{
					oldcount=0;
					USART_RX_CNT=0;
					Receive_Name();
					HAL_UART_Transmit(&huart1,(uint8_t*)rec,sizeof(rec),MAX_WAIT_TIME);				//���������ʾ
					MY_Write_Flash(Progress_Buff, add, PRO_MAX_NUM);													//������洢��FLASH
					HAL_UART_Transmit(&huart1,(uint8_t*)update,sizeof(update),MAX_WAIT_TIME);	//�̼����������ʾ
					break;
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
static void Flash_Execute_Progress(void)
{
	/*--------------------------ָ�-------------------------*/
	char start[] = "��ʼִ�г���!\r\n";
	char success[] = "����д��оƬ�ɹ�!/r/n";
	char error[] = "��������,�޷�ִ��!\r\n";
	/*--------------------------------------------------------*/
	uint32_t add;	//Ҫִ�еĳ���洢��ַ
	add = Pro_Address(Choose_Pro(1));																							//�����������˻�ȡ�������,��������Ż�ȡ�˳���洢��FLASH�ĵ�ַ
	MY_Read_Flash(Progress_Buff,add,PRO_SIZE);
	if(((*(__IO uint32_t*)(0X20001000+4))&0xFF000000)==0x08000000)								//�ж��Ƿ�Ϊ0X08XXXXXX.
	{	 
		iap_write_appbin(FLASH_APP1_ADDR,Progress_Buff,PRO_SIZE);										//�����ڲ�FLASH����   
		HAL_UART_Transmit(&huart1,(uint8_t*)success,sizeof(success),MAX_WAIT_TIME);	//���͸��������ʾ	
		if(((*(__IO uint32_t*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)					//�ж��Ƿ�Ϊ0X08XXXXXX.
		{	 
			HAL_UART_Transmit(&huart1,(uint8_t*)start,sizeof(start),MAX_WAIT_TIME);		//ִ�г�����ʾ
			iap_load_app(FLASH_APP1_ADDR);																						//ִ��FLASH APP����
		}
		else
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);		//������ʾ
		}
	}
	else
	{
		HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);			//������ʾ
	}

}


/*------------------------------------------------API����------------------------------------------*/
void Flash_Interaction(uint8_t flag)
{
	if(flag == 0) Flash_Hardware_Update();
	else Flash_Execute_Progress();
}
