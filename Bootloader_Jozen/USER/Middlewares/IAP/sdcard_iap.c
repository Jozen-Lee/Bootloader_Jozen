#include "sdcard_iap.h"

/* �������� -----------------------------------------------------*/
FATFS fatfs;						//�ļ�ϵͳ����
FIL fil;			    			//�ļ�����
UINT br;                //�ļ���ȡ���ֽ���
FRESULT rc;			    		//�ļ��������
char  File_Name[] = "0:UPDATE/update_app.bin";

/*---------------------------------------------------�ڲ�����---------------------------------------*/

//SD����ʼ������
static uint8_t SD_File_Init(void)
{        
  /* ע����̹����ռ� */
	rc = f_mount(&fatfs, "0:", 1);
	
	 
	/* ������ */
	if(rc != FR_OK && rc != FR_EXIST)		
	{
			f_close(&fil);          
					return 1;
	}			 
		
	/* �򿪽�Ҫ���µĴ����ļ� */
	 rc = f_open(&fil, (const TCHAR*)File_Name, FA_READ); 
	
	/* ������ */
	if(rc != FR_OK)              
	{
			f_close(&fil);       
			return 1;    	
	}		
	
	return 0;
}

//�̼����º���
static void SD_Hardware_Update(void)
{
	char error[] = "SD����������,����м��\r\n";
	//�Ƚ���SD���ĳ�ʼ��
	if(!SD_File_Init())
	{
		rc = f_read(&fil, Progress_Buff, PRO_MAX_SIZE, &br);
		if(rc != FR_OK)              
		{
				HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);	//������ʾ      	
		}	
		f_close(&fil);
	}
}

//ִ�г�����
static void SD_Execute_Progress(void)
{
	/*--------------------------ָ�-------------------------*/
	char start[] = "��ʼִ�г���!\r\n";
	char success[] = "����д��оƬ�ɹ�!\r\n";
	char error[] = "��������,�޷�ִ��!\r\n";
	/*--------------------------------------------------------*/
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
void SDcard_Interaction(uint8_t flag)
{
	SD_Hardware_Update();
	SD_Execute_Progress();	
}



