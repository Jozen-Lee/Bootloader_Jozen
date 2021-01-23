#include "sdcard_iap.h"

/* 变量定义 -----------------------------------------------------*/
FATFS fatfs;						//文件系统对象
FIL fil;			    			//文件对象
UINT br;                //文件读取的字节数
FRESULT rc;			    		//文件操作结果
char  File_Name[] = "0:UPDATE/update_app.bin";

/*---------------------------------------------------内部函数---------------------------------------*/

//SD卡初始化函数
static uint8_t SD_File_Init(void)
{        
  /* 注册磁盘工作空间 */
	rc = f_mount(&fatfs, "0:", 1);
	
	 
	/* 出错处理 */
	if(rc != FR_OK && rc != FR_EXIST)		
	{
			f_close(&fil);          
					return 1;
	}			 
		
	/* 打开将要更新的代码文件 */
	 rc = f_open(&fil, (const TCHAR*)File_Name, FA_READ); 
	
	/* 出错处理 */
	if(rc != FR_OK)              
	{
			f_close(&fil);       
			return 1;    	
	}		
	
	return 0;
}

//固件更新函数
static void SD_Hardware_Update(void)
{
	char error[] = "SD卡程序有误,请进行检查\r\n";
	//先进行SD卡的初始化
	if(!SD_File_Init())
	{
		rc = f_read(&fil, Progress_Buff, PRO_MAX_SIZE, &br);
		if(rc != FR_OK)              
		{
				HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);	//错误提示      	
		}	
		f_close(&fil);
	}
}

//执行程序函数
static void SD_Execute_Progress(void)
{
	/*--------------------------指令集-------------------------*/
	char start[] = "开始执行程序!\r\n";
	char success[] = "程序写入芯片成功!\r\n";
	char error[] = "程序有误,无法执行!\r\n";
	/*--------------------------------------------------------*/
	if(((*(__IO uint32_t*)(0X20001000+4))&0xFF000000)==0x08000000)								//判断是否为0X08XXXXXX.
	{	 
		iap_write_appbin(FLASH_APP1_ADDR,Progress_Buff,PRO_SIZE);										//更新内部FLASH代码   
		HAL_UART_Transmit(&huart1,(uint8_t*)success,sizeof(success),MAX_WAIT_TIME);	//发送更新完成提示	
		if(((*(__IO uint32_t*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)					//判断是否为0X08XXXXXX.
		{	 
			HAL_UART_Transmit(&huart1,(uint8_t*)start,sizeof(start),MAX_WAIT_TIME);		//执行程序提示
			iap_load_app(FLASH_APP1_ADDR);																						//执行FLASH APP代码
		}
		else
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);		//错误提示
		}
	}
	else
	{
		HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);			//错误提示
	}
}


/*------------------------------------------------API函数------------------------------------------*/
void SDcard_Interaction(uint8_t flag)
{
	SD_Hardware_Update();
	SD_Execute_Progress();	
}



