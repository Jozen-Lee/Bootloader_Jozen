#include "flash_iap.h"
#include "stdio.h"

/*-----------------------------辅助性函数-------------------------------*/
//返回对应的程序名存储地址
//参数:程序的序号
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

//返回对应的程序存储地址
//参数:程序的序号
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

//返回对应的程序名长度
//参数:存储程序名的数组指针
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


//写FLASH函数,满足写入的程序过长时的写入FLASH操作
static void MY_Write_Flash(uint8_t* pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite)
{
	uint8_t num;	//程序要分段的数目
	uint8_t temp;
	/*分两种情况处理*/
	
	/*1.写入的数据长度足够短*/
	if(NumByteToWrite <= 0xFFFF) SPI_Flash_Write(pBuffer, WriteAddr, NumByteToWrite);
	
	/*2.写入的数据长度过长*/
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

//读LASH函数,满足读取的程序过长时的读取FLASH操作
static void MY_Read_Flash(uint8_t* pBuffer,uint32_t ReadAddr,uint32_t NumByteToRead)
{
	uint8_t num;	//程序要分段的数目
	uint8_t temp;
	/*分两种情况处理*/
	
	/*1.写入的数据长度足够短*/
	if(NumByteToRead <= 0xFFFF) SPI_Flash_Read(pBuffer, ReadAddr, NumByteToRead);
	
	/*2.写入的数据长度过长*/
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

/*--------------------------------------------内部函数-------------------------------------------------*/
//接收程序名
static void Receive_Name(void)
{
	/*--------------------------指令集-------------------------*/
	char input_name[] = "请为所发送的程序命名!\r\n";
	/*--------------------------------------------------------*/
	uint32_t oldcount;
	USART_RX_CNT = PRO_SIZE;				//在程序后面的位置存储程序名
	oldcount = USART_RX_CNT;
	HAL_UART_Transmit(&huart1,(uint8_t*)input_name,sizeof(input_name),MAX_WAIT_TIME);
	while(1)
	{
		if(USART_RX_CNT != oldcount) break;	//说明开始接收数据
		HAL_Delay(10);
	}
	while(1)
	{
		if(oldcount==USART_RX_CNT)	//新周期内,没有收到任何数据,认为本次数据接收完成.
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

//清空程序缓存区,并将计数器置0
static void Clear_Buff(void)
{
	USART_RX_CNT = 0;
	for(uint32_t i = 0; i < PRO_MAX_SIZE; i++)
	{
		Progress_Buff[i] = NULL;
	}
}

//从FLASH中读取程序名,并通过串口发送出去
static void Read_Pro_Name_Flash(void)
{
	char name[NAME_SIZE];
	char num[] = "0. ";
	char empty[] = "空!\r\n";
	uint32_t add;					//程序名存储地址
	uint16_t name_length;	//程序名长度
	for(uint8_t i = 0; i < PRO_MAX_NUM; i++)
	{
		sprintf(num, "%d. ", i+1);
		add = Pro_Name_Address(i+1);																							//获取程序存储地址
		SPI_Flash_Read((uint8_t*)name, add, NAME_SIZE);														//从FLASH中读取程序
		name_length = Pro_Name_Length(name);																			//获取程序名长度
		name[name_length] = '\r'; name[name_length+1] = '\n';
		HAL_UART_Transmit(&huart1,(uint8_t*)num,sizeof(num),MAX_WAIT_TIME);				//打印程序序号
		if(name[0] != NULL) 
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)name,name_length+2,MAX_WAIT_TIME);	//有程序则打印程序名
		}	
		else 
		{
			HAL_UART_Transmit(&huart1,(uint8_t*)empty,sizeof(empty),MAX_WAIT_TIME);	//没有程序则打印空
		}
	}
}

////清理FLASH中所选择的程序
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

//选择所要访问的程序序号
//返回值:需要访问的程序序号
//参数: 0 写入外部FLASH, 1 写入内部FLASH
static uint8_t Choose_Pro(uint8_t mode)
{
	uint16_t oldcount = 0;
	uint8_t choice = 0, temp = 0;
	char inquiry0[] = "请选择需要覆盖的程序序号!\r\n";
	char inquiry1[] = "请选择需要运行的程序序号!\r\n";
	if(mode == 0) HAL_UART_Transmit(&huart1,(uint8_t*)inquiry0,sizeof(inquiry0),MAX_WAIT_TIME);
	else HAL_UART_Transmit(&huart1,(uint8_t*)inquiry1,sizeof(inquiry1),MAX_WAIT_TIME);
	Read_Pro_Name_Flash();		//打印程序名
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

//固件更新函数
static void Flash_Hardware_Update(void)
{
	/*--------------------------指令集-------------------------*/
	char update[] = "固件更新完成!\r\n";
	char rec[] = "数据接收完成\r\n";
	char trans[] = "请使用串口发送.bin文件!\r\n";	
	char error[] = "发送数据有误,请选择发送.bin文件\r\n";
	/*--------------------------------------------------------*/
	uint16_t oldcount=0;							//老的串口接收数据值
	uint32_t add;											//写入FLASH的地址
	add = Pro_Address(Choose_Pro(0));	//这句命令包括了获取程序序号,并根据序号获取了要写入FLASH的地址
	HAL_UART_Transmit(&huart1,(uint8_t*)trans,sizeof(trans),MAX_WAIT_TIME);
	Clear_Buff();
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)	//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				if(USART_RX_CNT > 50)	//防止误操作
				{
					oldcount=0;
					USART_RX_CNT=0;
					Receive_Name();
					HAL_UART_Transmit(&huart1,(uint8_t*)rec,sizeof(rec),MAX_WAIT_TIME);				//传输完成提示
					MY_Write_Flash(Progress_Buff, add, PRO_MAX_NUM);													//将程序存储进FLASH
					HAL_UART_Transmit(&huart1,(uint8_t*)update,sizeof(update),MAX_WAIT_TIME);	//固件更新完成提示
					break;
				}
				else
				{
					USART_RX_CNT=0;
					HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);	//错误提示
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

//执行程序函数
static void Flash_Execute_Progress(void)
{
	/*--------------------------指令集-------------------------*/
	char start[] = "开始执行程序!\r\n";
	char success[] = "程序写入芯片成功!/r/n";
	char error[] = "程序有误,无法执行!\r\n";
	/*--------------------------------------------------------*/
	uint32_t add;	//要执行的程序存储地址
	add = Pro_Address(Choose_Pro(1));																							//这句命令包括了获取程序序号,并根据序号获取了程序存储在FLASH的地址
	MY_Read_Flash(Progress_Buff,add,PRO_SIZE);
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
void Flash_Interaction(uint8_t flag)
{
	if(flag == 0) Flash_Hardware_Update();
	else Flash_Execute_Progress();
}
